#ifndef STrig
#define STrig

// https://www.gamedev.net/forums/topic/621589-extremely-fast-sin-approximation/
inline int fast_round(double x) {
    /*const double MAGIC_ROUND = 6755399441055744.0; // http://stereopsis.com/sree/fpu2006.html 

    union {
    double d;

    struct {
    int lw;
    int hw;
    };
    } fast_trunc;

    fast_trunc.d = x;
    fast_trunc.d += MAGIC_ROUND;

    return fast_trunc.lw;*/

    if (x < 0)
        return (int) (x - 0.5);
    return (int) (x + 0.5);
    

}

inline double tsin(double x) {
    const double PI = 3.14159265358979323846264338327950288;
    const double INVPI = 0.31830988618379067153776752674502872;
    const double A = 0.00735246819687011731341356165096815;
    const double B = -0.16528911397014738207016302002888890;
    const double C = 0.99969198629596757779830113868360584;

    int k;
    double x2;

    /* find offset of x from the range -pi/2 to pi/2 */
    k = fast_round(INVPI * x);

    /* bring x into range */
    x -= k * PI;

    /* calculate sine */
    x2 = x * x;
    x = x*(C + x2*(B + A*x2));

    /* if x is in an odd pi count we must flip */
    if (k % 2) x = -x;

    return x;
}

inline double tcos(double x) {
    const double PI = 3.14159265358979323846264338327950288;
    return tsin(x + PI/2);
}


#endif