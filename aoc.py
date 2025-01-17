import functools

@functools.cache
def count( n, b ):
    if b == 75:
        return 1
    if n == 0:
        return count( 1, b + 1 )
    ns = str( n )
    nl = len( ns )
    if nl & 1 == 0:
        return ( count( int( ns[ : nl // 2 ] ), b + 1 ) +
                 count( int( ns[ nl // 2 : ] ), b + 1 ) )
    return count( n * 2024, b + 1 )

print( sum( count( int( n ), 0 ) for n in open( 0 ).read().split() ) )