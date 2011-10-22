#include <math.h>

namespace general_module
{
	template< class T, int X_MIN, int X_MAX, int Y_MIN, int Y_MAX >
	bool is_in_space( const T x, const T y )
	{
		return ( X_MIN < static_cast< int >( x ) ) && ( X_MAX > static_cast< int >( x ) ) && \
			( Y_MIN < static_cast< int >( y ) ) && ( Y_MAX > static_cast< int >( y ) );
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
}