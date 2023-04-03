import gtpsa
import numpy as np


def identity_with_zeros(mat):
    """accept rectangular shapes"""
    n = min(mat.shape)
    id = np.identity(n)
    r = np.zeros(mat.shape, mat.dtype)
    r[:n, :n] = id
    return r


def test_jacobian_with_without_knobs():
    nv = 9
    nk = 3
    mo = 3
    n = 7
    desc = gtpsa.desc(nv, mo)
    desc2 = gtpsa.desc(nv, mo, nk, 2)

    v1 = gtpsa.ss_vect_tpsa(desc, mo, n)
    v1.set_identity()
    v2 = gtpsa.ss_vect_tpsa(desc2, mo, n)
    v2.set_identity()

    jac1 = v1.jacobian()
    jac2 = v2.jacobian()

    print(jac1)
    print(jac2)

    del v1
    del v2
    jac1 = np.array(jac1)
    jac2 = np.array(jac2)
    I = identity_with_zeros(jac1)

    print(I)
    print(jac1)
    print(jac2)
    diff = jac1 - jac2

    adiff = np.absolute(diff)
    test = np.sum(np.sum(adiff))
    assert test < 1e-12

    assert np.sum(np.sum(np.absolute(jac1 - I))) < 1e-1
    assert np.sum(np.sum(np.absolute(jac2 - I))) < 1e-1


if __name__ == "__main__":
    test_jacobian_with_without_knobs()
