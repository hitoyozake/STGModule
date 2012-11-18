#include "direct_x_include.h"
#include "standard_include.h"
#include "directx_class_define.h"
#include "boost.h"

namespace direct_x_settings
{

	//テクスチャの取得と開放
	class texture_manager
	{
	public:

	private:
		texture_manager()
		{
		};

		boost::optional< int > create_texture( std::string const & filename )
		{
			//取得

			return boost::optional< int > ();//失敗時
		}



	};
}