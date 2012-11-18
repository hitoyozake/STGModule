#pragma once

#include "standard_include.h"
#include "direct_x_include.h"

#include "boost.h"


namespace direct_x_audio
{
	class audio
	{
	private:
		struct sound_buffer
		{
			LPDIRECTSOUNDBUFFER	sound_buf_;
		};

	public:
		LPDIRECTSOUND8 sound_dev_;
		std::vector< sound_buffer > sound_buffer_;
		//loadしてポインタを返す、的な?

		//外部公開用。こいつのポインタを返す
		//autoでもポインタ取れるけれど、いろいろとアレなのでこっちを使う
		struct sound_buffer_pointer
		{
			sound_buffer * const buf_;

			sound_buffer_pointer( sound_buffer * const buf ) : buf_( buf )
			{}

			sound_buffer const & buf() const //&をつけて左辺値の時にしか呼べないようにすると安全?
			{
				return ( * buf_ );
			}
		};

		audio( const HWND hwnd )
		{
			//Create SoundDevice
			DirectSoundCreate8( NULL, & sound_dev_, NULL );
			sound_dev_->SetCooperativeLevel( hwnd, DSSCL_PRIORITY );
		}

		boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > > \
			open_wave( std::string const & filename ) const
		{
			HMMIO hmmio = NULL;
			MMIOINFO mmio_info;

			//open wave file
			memset( & mmio_info, 0, sizeof MMIOINFO );
			char str[ 256 ];
			strcpy_s( str, filename.c_str() );
			hmmio = mmioOpen( str, & mmio_info, MMIO_READ );

			if( !hmmio )
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();

			//search RIFF chunk
			MMRESULT mm_res;
			MMCKINFO riff_chunk;

			riff_chunk.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
			mm_res = mmioDescend( hmmio, & riff_chunk, NULL, MMIO_FINDRIFF );

			if( mm_res != MMSYSERR_NOERROR )
			{
				mmioClose( hmmio, 0 );
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();
			}

			// フォーマットチャンク検索---------------------
			MMCKINFO format_chunk;
			format_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
			mm_res = mmioDescend( hmmio, & format_chunk, & riff_chunk, MMIO_FINDCHUNK );
			if( mm_res != MMSYSERR_NOERROR )
			{
				mmioClose( hmmio, 0 );
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();
			}


			DWORD fmsize = format_chunk.cksize;
			WAVEFORMATEX wave_formatex;
			DWORD size = mmioRead( hmmio, ( HPSTR ) & wave_formatex, fmsize );
			if( size != fmsize )
			{
				mmioClose( hmmio, 0 );
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();
			}

			mmioAscend( hmmio, & format_chunk, 0 );

			// データチャンク検索---------------------
			MMCKINFO data_chunk;
			data_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
			mm_res = mmioDescend( hmmio, &data_chunk, & riff_chunk, MMIO_FINDCHUNK );
			if( mm_res != MMSYSERR_NOERROR )
			{
				mmioClose( hmmio, 0 );
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();
			}

			std::string ppdata;
			ppdata.resize( data_chunk.cksize, '\0' );

			size = mmioRead( hmmio, static_cast< HPSTR >( & ppdata[ 0 ] ), data_chunk.cksize );
			if(size != data_chunk.cksize ) {
				return boost::optional< std::tuple<  WAVEFORMATEX, DWORD, std::string > >();
			}

			// ハンドルクローズ
			mmioClose( hmmio, 0 );

			return boost::optional< std::tuple< WAVEFORMATEX, DWORD, std::string > >\
				( std::tuple< WAVEFORMATEX, DWORD, std::string >( wave_formatex, size, std::move( ppdata ) ) );
		}

		sound_buffer_pointer load_wav( std::string const & filename )
		{
			DWORD wave_size = 0;

			if( auto const info = open_wave( filename ) )
			{
				enum tuple_info
				{
					E_WAVE_FORMATEX,
					E_WAVE_SIZE,
					E_PWAVE_DATA,
				};

				//データを取り出す
				auto fxformat = std::move( std::get< E_WAVE_FORMATEX >( * info ) );
				DSBUFFERDESC ds_buffer_desc;
				ds_buffer_desc.dwSize = sizeof(DSBUFFERDESC);
				ds_buffer_desc.dwFlags = 0;
				ds_buffer_desc.dwBufferBytes = std::get< E_WAVE_SIZE >( * info );
				ds_buffer_desc.dwReserved = 0;
				ds_buffer_desc.lpwfxFormat = & fxformat;
				ds_buffer_desc.guid3DAlgorithm = GUID_NULL;

				//サウンドバッファを作成
				sound_buffer sound_buf;
				//Create secondary buffer
				sound_dev_->CreateSoundBuffer( & ds_buffer_desc, & sound_buf.sound_buf_, NULL );

				//Write sound data to buffer

				LPVOID lp_buf_lock = 0;
				DWORD dw_length = 0;

				if( DS_OK == sound_buf.sound_buf_->Lock( 0, 0,\
					& lp_buf_lock, & dw_length, \
					NULL, NULL, DSBLOCK_ENTIREBUFFER ) )
				{
					//copy wave data
					memcpy( lp_buf_lock, std::get< E_PWAVE_DATA >( * info ).c_str(), dw_length );
					//unlock
					sound_buf.sound_buf_->Unlock( lp_buf_lock, dw_length, NULL, 0 );

					sound_buffer_.push_back( std::move( sound_buf ) );
					//ここでポインタ + boost::optionalを返すべき or nullptr
					return sound_buffer_pointer( & sound_buffer_.back() );
				}
				else
				{
					//失敗時の処理

					return sound_buffer_pointer( nullptr );
				}

			}
			return sound_buffer_pointer( nullptr );
		}

		void play_sound( sound_buffer const & buf )
		{
			buf.sound_buf_->Play( 0, 0, 0);
			//第三引数はループとかのフラグ
		}



	};
}