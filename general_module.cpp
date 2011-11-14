#include <math.h>

namespace general_module
{
	struct position
	{
		double x_, y_;
	};

	template< class T, class position, int X_MIN, int X_MAX, int Y_MIN, int Y_MAX >
	bool is_in_space( const position & pos )
	{
		return ( X_MIN < static_cast< int >( pos.x_ ) ) && ( X_MAX > static_cast< int >( pos.x_ ) ) && \
			( Y_MIN < static_cast< int >( pos.y_ ) ) && ( Y_MAX > static_cast< int >( pos.y_ ) );
	}

	template< int X_MIN, int X_MAX, int Y_MIN, int Y_MAX >
	bool is_in_space( const positoin & pos )
	{
		return ( X_MIN <  static_cast< int >( pos.x_ ) ) && ( X_MAX > static_cast< int >( pos.x_ ) ) && \
			( Y_MIN < static_cast< int >( pos.y_ ) ) && ( Y_MAX > static_cast< int >( pos.y_ ) );
	}

	template< class T >
	bool is_hit( const T x1, const T y1, const T x2, const T y2, const T r )
	{
		const auto x = abs( x1 - x2 );
		const auto y = abs( y1 - y2 );

		if( y <= r && x <= r )
		{
			if( x * x + y * y <= r * r )
			{
				return true;
			}
			return false;
		}
		else
			return false;
	}

	template< class T >
	double get_angle( const T from_x, const T from_y, \
		const T to_x, const T to_y )
	{
		return atan2( static_cast< double >( to_y - from_y ), \
			static_cast< double >( to_x - from_x ) );
	}

	template<>
	double get_angle( const double from_x, const double from_y, \
		const double to_x, const double to_y )
	{
		return atan2( to_y - from_y, to_x - from_x );
	}

}