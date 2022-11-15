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

@pytest.mark.skip
def test_02_set_tpsa_cst():
    """Set constant part of a space state containing tpsa

    __getitem__ essentially returns a new object not
    an object referencing to the internal object.

    Thus assignment will not change the internal object
    currently
    """

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)

    ss1[1].set(0, 2)
    assert(ss1[1].get() == 2)
    assert(ss1.cst()[1] == 2)


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

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)

    a_tpsa_obj= ss1[1]
    a_tpsa_obj.set(0, 2)
    ss1[1] = a_tpsa_obj
    del a_tpsa_obj

    assert(ss1[1].get() == 2)
    assert(ss1.cst()[1] == 2)


def test_09_ss_vect_from_double_vect():
    vec = np.array([1, 2, 3, 4, 5, 6], np.double)
    ss_vect = gtpsa.ss_vect_double(vec)

    assert ( ss_vect[0] == 1 )
    assert ( ss_vect[1] == 2 )
    assert ( ss_vect[2] == 3 )
    assert ( ss_vect[3] == 4 )
    assert ( ss_vect[4] == 5 )
    assert ( ss_vect[5] == 6 )


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


def test_12_radd_double():
    """add a double value to a double ss vector

    Currently this radd fails and returns None!
    """
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.name = "test_12_radd_double"

    ss1.set_zero()
    ss1 += 2.0

    assert(ss1 is not None)

    assert(ss1[1] == 2)
    assert(ss1[3] == 2)


def test_20_add_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss1[1] = 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss2[3] = 5
    ss3 = ss1 + ss2

    assert(ss3[1] == 2)
    assert(ss3[3] == 5)


def test_21_radd_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss1[1] = 2

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss2[3] = 5
    ss1 += ss2

    assert(ss1[1] == 2)
    assert(ss1[3] == 5)


def test_22_sub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss1[1] = 2
    ss2[3] = 5

    ss3 = ss1 - ss2

    assert(ss3[1] == 2)
    assert(ss3[3] == -5)


def test_23_rsub_double():
    d = 0.0
    ss1 = gtpsa.ss_vect_double(d)
    ss1.set_zero()

    ss2 = gtpsa.ss_vect_double(d)
    ss2.set_zero()

    ss1[1] = 2
    ss2[3] = 5

    ss1 -= ss2

    assert(ss1[1] == 2)
    assert(ss1[3] == -5)


@pytest.mark.skip
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
    ss1[1].set(0, 2)

    ss2[3] = 5

    assert(ss1[1].get() == 2)
    assert(ss2[3] == 5)

    print("Adding values")
    ss1 += ss2
    print("Adding values")

    assert(ss1 is not None)

    print(ss1.cst())
    assert(ss1[1] == 2)
    assert(ss1[3] == 5)


if __name__ == "__main__":
    test_09_ss_vect_from_double_vect()


if __name__ == "__main__":
    test_12_radd_double()


if __name__ == "__main__":
    test_03_set_tpsa_cst_object()


if __name__ == "__main__":
    test_02_set_tpsa_cst()


if __name__ == "__main__":
    test_30_tpsa_radd_double()
