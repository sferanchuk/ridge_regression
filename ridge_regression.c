
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define max_n 50
#define max_m max_n
#define max_e 150

void elnet_( int *ka, double *parm, int *no, int *ni, double *x, double *y, double *w, int *jd, double *vp, double *cl, int *ne, int *nx, int *nlam, double *flmin, double *ulam, double *thr, int *isd, int *intr, int *maxit, int *lmu, double *a0, double *ca, int *ia, int *nin, double *rsq, double *alm, int *nlp, int *jerr ); 

void solns_( int *ni, int *nx, int *lmu, double *ca, int *ia, int *nin, double *b );

int main( int argc, char **argv )
{
    int i,j;
    char *p;
    char buf[4096];
    int m, n;
    int ka = 2, no, ni, jd[ max_n + 1 ], ne, nx, nlam, isd, intr, maxit = 100000, lmu, ia[ max_e ], nin[ max_e ], nlp, jerr;
    double parm, x[ max_m * max_n ], y[ max_n ], w[ max_n ], vp[ max_n ], cl[ 2 * max_m ], flmin, ulam[ max_e ], thr = 1e-5, a0[ max_m ], ca[ max_n * max_e ], rsq[ max_e ], alm[ max_m ], b[ max_n * max_e ];

    if ( argc == 1 )
    {
        printf( "arguments: num_observations num_parameters observations weights lower_bounds upper_bounds\nmatrix is read from stdin\nall vectors are assumed to be comma-separated;\n\nparm, flmin, ulam0 are expected in the environment." );
        return 1;
    }
    
    m = atoi( argv[1] );
    n = atoi( argv[2] );
    if ( m >= max_m || n >= max_n ) return 1;
    
    no = m;
    ni = n;
    ne = max_e;
    nx = m;
    nlam = m;
    isd = 1;
    intr = 0;
    
    jd[0] = 0;
    parm = atof( getenv( "parm" ) );
    flmin = atof( getenv( "flmin" ) );
    ulam[0] = atof( getenv( "ulam0" ) );
    
    for ( i = 0; i < no; i++ )
    {
        fgets( buf, 4096, stdin );
        for ( p = strtok( buf, "," ), j=0; p && j < ni; p = strtok( 0, "," ), j++ ) x[ ni * i + j ] = atof( p );
    }
    
    strcpy( buf, argv[3] ); for ( p = strtok( buf, "," ), i=0; p && i < no; p = strtok( 0, "," ), i++ ) y[i] = atof( p ); for( ; i < no; i++ ) y[i] = 0;
    strcpy( buf, argv[4] ); for ( p = strtok( buf, "," ), i=0; p && i < no; p = strtok( 0, "," ), i++ ) w[i] = atof( p ); for( ; i < no; i++ ) w[i] = w[0];
    strcpy( buf, argv[5] ); for ( p = strtok( buf, "," ), i=0; p && i < ni; p = strtok( 0, "," ), i++ ) cl[i] = atof( p ); for( ; i < ni; i++ ) cl[i] = cl[0];
    strcpy( buf, argv[6] ); for ( p = strtok( buf, "," ), i=0; p && i < ni; p = strtok( 0, "," ), i++ ) cl[ni+i] = atof( p ); for( ; i < ni; i++ ) cl[ni+i] = cl[ni];
    for ( i  = 0; i < ni; i++ ) vp[i] = 1;
    
    elnet_( &ka, &parm, &no, &ni, x, y, w, jd, vp, cl, &ne, &nx, &nlam, &flmin, ulam, &thr, &isd, &intr, &maxit, &lmu, a0, ca, ia, nin, rsq, alm, &nlp, &jerr ); 
    if ( jerr != 0 ) { printf( "jerr = %d\n", jerr ); return 1; }
    solns_( &ni, &nx, &lmu, ca, ia, nin, b );

    for ( i = 0; i < lmu; i++ )
        for ( j = 0; j < ni; j++ ) printf( "%g%c", b[ ni * i + j ], ( j + 1 < ni ) ? ',' : '\n' );
}
