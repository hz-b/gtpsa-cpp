import pytest
import numpy as np
import gtpsa

def test_1_get_set():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)

    t1.set(0, 2)

    assert t1.get() == 2


def test_11_radd_double():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)

    t1 += 2

def test_20_radd():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)

    # Check if lifetime of desc is correctly handled ...
    del desc

    a = np.pi / 6
    v = np.array([a, 0, 1, 0, 0, 0, 0])
    t1.setv(0, v)

    b = 5
    t1 += b

    assert t1.get() == pytest.approx(a + b, abs=1e-10)

def test_21_add():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)
    del desc

    a = np.pi / 6
    v = np.array([a, 0, 1, 0, 0, 0, 0])
    t1.setv(0, v)

    b = 5
    t2 = t1 + b

    assert t2.get() == pytest.approx(a + b, abs=1e-30)


def test_22_rsub():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)
    del desc

    a = np.pi / 6
    v = np.array([a, 0, 1, 0, 0, 0, 0])
    t1.setv(0, v)

    b = 5
    t1 -= b

    assert t1.get() == pytest.approx(a - b)

def test_23_sub():
    desc = gtpsa.desc(nv=6, mo=4)
    t1 = gtpsa.tpsa(desc, 3)
    del desc

    a = np.pi / 6
    v = np.array([a, 0, 1, 0, 0, 0, 0])
    t1.setv(0, v)

    b = 5
    t2 = t1 - b

    assert t2.get() == pytest.approx(a  - b)


if __name__ == "__main__":
    test_20_radd()
