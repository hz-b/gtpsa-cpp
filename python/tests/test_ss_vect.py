import pytest
import numpy as np
import gtpsa

def test_00_init_double():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    ss.setZero()

def test_01_init_tpsa():
    desc = gtpsa.desc(6, 4)
    t1 = gtpsa.tpsa(desc, 1)
    ss = gtpsa.ss_vect_tpsa(t1)

    ss.setIdentity()


def test_10_index():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    ss[0] = 2
    ss[2] = 4
    ss[4] = 6

    print(ss.cst() + 1)

    print(ss.cst() )

def test_11_index_off_range():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    with pytest.raises(IndexError):
        ss[len(ss)] = 2

def test_20_add_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.setZero()

    ss1[1] = 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.setZero()

    ss2[3] = 5
    ss3 = ss1 + ss2

    assert(ss3[1] == 2)
    assert(ss3[3] == 5)

def test_21_radd_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.setZero()

    ss1[1] = 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.setZero()

    ss2[3] = 5
    ss1 += ss2

    assert(ss1[1] == 2)
    assert(ss1[3] == 5)

def test_22_sub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.setZero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.setZero()

    ss1[1] = 2
    ss2[3] = 5

    ss3 = ss1 - ss2

    assert(ss3[1] == 2)
    assert(ss3[3] == -5)

def test_23_rsub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.setZero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.setZero()

    ss1[1] = 2
    ss2[3] = 5

    ss1 -= ss2

    assert(ss1[1] == 2)
    assert(ss1[3] == -5)


if __name__ == "__main__":
    test_10_index()
