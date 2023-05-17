import pytest
import numpy as np
import gtpsa


def test_00_init_double():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    ss.set_zero()


def test_01_init_tpsa():
    desc = gtpsa.desc(6, 4)
    t1 = gtpsa.tpsa(desc, 1)
    ss = gtpsa.ss_vect_tpsa(t1)

    ss.set_identity()


#@pytest.mark.skip
def test_02_set_tpsa_cst():
    """Set constant part of a space state containing tpsa

    __getitem__ essentially returns a new object not
    an object referencing to the internal object.

    Thus assignment will not change the internal object
    currently
    """

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)

    py = ss1.py
    py.set(0, 2)
    assert py.get() == 2
    assert ss1.py.get() == 2

    ss1.iloc[1].set(0, 2)
    assert ss1.iloc[1].get() == 2
    assert ss1.cst().iloc[1] == 2


# @pytest.mark.skip
def test_03_set_tpsa_cst_object():
    """Set constant part of a space state containing tpsa

    getitem returns a new object not one that mangles the
    internal part

    Therefore you need to assign the returned object to a variable
    change it and then stuff it back into the ss vector

    Warning:
        Do not expect that the returned object will continue to be a new one
        Work in progress
    """
    import sys

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)
    print("ss1")
    sys.stdout.write("ss1\n")
    sys.stdout.flush()
    a_tpsa_obj = ss1[1]
    a_tpsa_obj.set(0, 2)
    ss1.iloc[1] = a_tpsa_obj
    del a_tpsa_obj

    assert ss1.iloc[1].get() == 2
    sys.stdout.write("ss1 got\n")
    sys.stdout.flush()

    t_cst = ss1.cst()
    sys.stdout.write("cst got\n")
    sys.stdout.flush()
    tmp = t_cst.iloc[1]
    assert tmp == 2
    sys.stdout.write("cst iloc got\n")
    sys.stdout.flush()
    assert ss1.cst().iloc[1] == 2


def test_09_ss_vect_from_double_vect():
    vec = np.array([1, 2, 3, 4, 5, 6], np.double)
    ss_vect = gtpsa.ss_vect_double(vec)

    assert ss_vect.iloc[0] == 1
    assert ss_vect.iloc[1] == 2
    assert ss_vect.iloc[2] == 3
    assert ss_vect.iloc[3] == 4
    assert ss_vect.iloc[4] == 5
    assert ss_vect.iloc[5] == 6


def test_10_index():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    ss.iloc[0] = 2
    ss.iloc[2] = 4
    ss.iloc[4] = 6

    print(ss.cst() + 1)

    print(ss.cst())


def test_11_index_off_range():
    d = 0.0
    ss = gtpsa.ss_vect_double(d)

    with pytest.raises(IndexError):
        ss.iloc[len(ss)] = 2


def test_12_radd_double():
    """add a double value to a double ss vector

    Currently this radd fails and returns None!
    """
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.name = "test_12_radd_double"

    ss1.set_zero()
    print("type ss1", type(ss1), ss1)
    ss1 += 2.0
    print("type ss1 (after adding float)", type(ss1), ss1)

    assert ss1 is not None

    assert ss1.iloc[1] == 2
    assert ss1.iloc[3] == 2

def test_15_tpsa_copy():
    desc = gtpsa.desc(6, 2)
    ps1 = gtpsa.ss_vect_tpsa(desc, 1)
    ps1.set_identity()
    ps2 = ps1.copy()
    ps1.set_zero()

    print(ps1)
    print(ps2)
    assert True == False

def test_20_add_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss1.iloc[1] = 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss2.iloc[3] = 5
    ss3 = ss1 + ss2
    print("type ss1", type(ss1))
    print("type ss3", type(ss3))
    assert ss3.iloc[1] == 2
    assert ss3.iloc[3] == 5


def test_21_radd_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss1.iloc[1] = 2
    assert ss1.iloc[1] == 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss2.iloc[3] = 5
    assert ss2.iloc[3] == 5
    print("type ss2", type(ss2), ss2)
    ss1 += ss2
    print("type ss1", type(ss1), ss1)
    assert ss1.iloc[1] == 2
    assert ss1.iloc[3] == 5


def test_22_sub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss1.iloc[1] = 2
    ss2.iloc[3] = 5

    ss3 = ss1 - ss2

    assert ss3.iloc[1] == 2
    assert ss3.iloc[3] == -5


def test_23_rsub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss1.iloc[1] = 2
    ss2.iloc[3] = 5

    ss1 -= ss2

    assert ss1.iloc[1] == 2
    assert ss1.iloc[3] == -5


# @pytest.mark.skip
def test_30_tpsa_radd_double():

    desc = gtpsa.desc(6, 1)

    ss1 = gtpsa.ss_vect_tpsa(desc, 1)
    ss1.set_zero()

    for i in range(6):
        t = ss1[i]
        t.uid = i
        ss1[i].name = "test_radd_double"

    d = 1.0
    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    # Intentionally not supporting it as in
    # python it would assign a reference to it normally
    # thus I think it will be inconsistent to the user
    # ss1[1] = 2
    # ss2[3] = 5
    # Equivalent form below
    ss1.iloc[1].set(0, 2)

    ss2.iloc[3] = 5

    assert ss1.iloc[1].get() == 2
    assert ss2.iloc[3] == 5

    print("Adding values")
    ss1 += ss2
    print("Adding values")

    assert ss1 is not None

    print(ss1.cst())
    assert ss1.iloc[1] == 2
    assert ss1.iloc[3] == 5


def test_50_compose():

    desc = gtpsa.desc(6, 1)

    ss1 = gtpsa.ss_vect_tpsa(desc, 1)
    ss1.set_zero()

    ss2 = gtpsa.ss_vect_tpsa(desc, 1)
    ss2.set_zero()

    I = np.identity(6)
    ss1.set_jacobian(I)
    ss2.set_jacobian(I)

    ss3 = gtpsa.compose(ss1, ss2)
    print(ss3)


def test70_sst_hessian():
    desc = gtpsa.desc(6, 3)
    ss_vect = gtpsa.ss_vect_tpsa(desc, 2)
    ss_vect.set_identity()
    for i, val in enumerate([1, 2, 3, 5, 7, 11]):
        ss_vect.iloc[i] += val

    print("type ss_vec", type(ss_vect))
    vec = ss_vect.copy()
    print("type vec", type(vec))
    # fmt: off
    vec.iloc[0] =  1 * ss_vect.iloc[0] * ss_vect.iloc[0];
    vec.iloc[1] =  2 * ss_vect.iloc[1] * ss_vect.iloc[1];
    vec.iloc[2] =  3 * ss_vect.iloc[2] * ss_vect.iloc[2];
    vec.iloc[3] =  5 * ss_vect.iloc[3] * ss_vect.iloc[3];
    vec.iloc[4] =  7 * ss_vect.iloc[4] * ss_vect.iloc[4];
    vec.iloc[5] = 11 * ss_vect.iloc[5] * ss_vect.iloc[5];
    # fmt: on

    hes = vec.hessian()
    assert type(hes) == np.ndarray

    assert hes[0, 0, 0] == 1
    assert hes[1, 1, 1] == 2
    assert hes[2, 2, 2] == 3
    assert hes[3, 3, 3] == 5
    assert hes[4, 4, 4] == 7
    assert hes[5, 5, 5] == 11

    hes[0, 0, 0] = 0
    hes[1, 1, 1] = 0
    hes[2, 2, 2] = 0
    hes[3, 3, 3] = 0
    hes[4, 4, 4] = 0
    hes[5, 5, 5] = 0

    assert np.sum(hes).all() == 0


named_index_d = dict(x=0, px=1, y=2, py=3, delta=4, ct=5, K=6, dx=7, dy=8)
named_index = gtpsa.IndexMapping(named_index_d)
def test100_named_access():
    nv = 6
    desc = gtpsa.desc(6, 2, 3, 1)
    ps = gtpsa.ss_vect_tpsa(desc, 2, index_mapping=named_index)
    t = ps.loc["px"]
    # Check that K can be accessed
    t.get(K=1)

    td = t.deriv("x")
    print(type(td))
    td.get(K=1)


## if __name__ == "__main__":
##     test70_sst_hessian()
##
## if __name__ == "__main__":
##     test_50_compose()
##
## if __name__ == "__main__":
##     test_09_ss_vect_from_double_vect()
##
##
## if __name__ == "__main__":
##     test_12_radd_double()
##
##
if __name__ == "__main__":
    test_03_set_tpsa_cst_object()


## if __name__ == "__main__":
##     test_02_set_tpsa_cst()
##
##
## if __name__ == "__main__":
##     test_30_tpsa_radd_double()
