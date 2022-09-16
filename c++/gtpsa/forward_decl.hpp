#ifndef _GTPSA_FORWARED_DECL_H_
#define _GTPSA_FORWARED_DECL_H_ 1
namespace gtpsa {

    class tpsa;
    class desc;

    // forward declaration ... required that operators will use the same functions
    inline void  add (const tpsa& a, const tpsa& b, tpsa* r);
    inline void  dif (const tpsa& a, const tpsa& b, tpsa* r);
    inline void  sub (const tpsa& a, const tpsa& b, tpsa* r);
    inline void  mul (const tpsa& a, const tpsa& b, tpsa* r);
    inline void  div (const tpsa& a, const tpsa& b, tpsa* r);
    inline void  pow (const tpsa& a, const tpsa& b, tpsa* r);

    inline tpsa  add (const tpsa& a, const tpsa& b         );
    inline tpsa  dif (const tpsa& a, const tpsa& b         );
    inline tpsa  sub (const tpsa& a, const tpsa& b         );
    inline tpsa  mul (const tpsa& a, const tpsa& b         );
    inline tpsa  div (const tpsa& a, const tpsa& b         );

    inline void  add_d (const tpsa& a, const double b, tpsa* r);
    inline void  sub_d (const tpsa& a, const double b, tpsa* r);
    inline void  mul_d (const tpsa& a, const double b, tpsa* r);
    inline void  div_d (const tpsa& a, const double b, tpsa* r);
    inline void  pow_d (const tpsa& a, const double b, tpsa* r);


    inline tpsa  add_d (const tpsa& a, const double b          );
    inline tpsa  dif_d (const tpsa& a, const double b          );
    inline tpsa  sub_d (const tpsa& a, const double b          );
    inline tpsa  mul_d (const tpsa& a, const double b          );
    inline tpsa  div_d (const tpsa& a, const double b          );

    tpsa inline process2(const tpsa& a, const tpsa&   b, void (*func)(const tpsa& a, const tpsa&   b, tpsa *r) );
    tpsa inline process2(const tpsa& a, const double  b, void (*func)(const tpsa& a, const double  b, tpsa *r) );

}
#endif /* _GTPSA_FORWARED_DECL_H_ */
