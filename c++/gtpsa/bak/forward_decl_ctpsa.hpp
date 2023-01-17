#ifndef _GTPSA_FORWARED_DECL_CTPSA_H_
#define _GTPSA_FORWARED_DECL_CTPSA_H_ 1
namespace gtpsa {

    class tpsa;
    class ctpsa;
    class desc;

    /*-----------------------------------------------------------------------*/
    // ctpsa part
    inline void  add (const ctpsa& a, const ctpsa& b, ctpsa* r);
    inline void  dif (const ctpsa& a, const ctpsa& b, ctpsa* r);
    inline void  sub (const ctpsa& a, const ctpsa& b, ctpsa* r);
    inline void  mul (const ctpsa& a, const ctpsa& b, ctpsa* r);
    inline void  div (const ctpsa& a, const ctpsa& b, ctpsa* r);
    inline void  pow (const ctpsa& a, const ctpsa& b, ctpsa* r);

    ctpsa inline process2(const ctpsa& a, const ctpsa&   b, void (*func)(const ctpsa& a, const ctpsa&   b, ctpsa *r) );

} // namespace gtpsa

#endif /* _GTPSA_FORWARED_DECL_CTPSA_H_ */
