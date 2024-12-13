import pytest
import gtpsa
import numpy as np

nv = 6
mo = 2
n_pars = 3
po = 2
# a mapping for 3 variables
named_index_d = dict(x=0, px=1, y=2, py=3, delta=4, ct=5, K=6, dx=7, dy=8)
named_index = gtpsa.IndexMapping(named_index_d)

desc = gtpsa.desc(nv, mo, n_pars, po)



def test_set_var():
    """Test setting a variable"""
    val = 42
    var_index = 3
    t = gtpsa.tpsa(desc, 1)
    t.set_variable(val, var_index)
    print("t\n", t)
    assert t.get() == pytest.approx(val, 1e-12)
    derivs = t.getv(1)
    print("1. derivatives", derivs)
    assert derivs[var_index - 1] == pytest.approx(1, 1e-12)
    derivs[var_index - 1] = 0
    assert np.sum(np.absolute(derivs)) == pytest.approx(0, abs=1e-12)


def test_set_knob_as_var():
    """Test setting a knob using variable interface

    Will raise an error in the tpsa side
    """
    val = 42
    var_index = nv + 2
    t = gtpsa.tpsa(desc, 1)
    # check first will
    with pytest.raises(RuntimeError) as exc_info:
        t.set_variable(val, var_index, check_first=True)


def test_knob():
    """Test setting a knob"""
    val = 3
    knob_index = nv + 1
    t = gtpsa.tpsa(desc, 1)
    t.set_knob(val, knob_index)
    print("t\n", t)
    assert t.get() == pytest.approx(3, 1e-12)
    derivs = t.getv(1)
    print("1. derivatives", derivs)
    assert derivs[knob_index] == pytest.approx(1, 1e-12)
    derivs[knob_index] = 0
    assert np.sum(np.absolute(derivs)) == pytest.approx(0, abs=1e-12)

def test_index_mapping():
    names = dir(named_index)
    assert len(names) == len(list(named_index_d.keys()))


def test_powers_dict_not_overlapping():
    """test how powers are generated if a dict and kwargs are provided"""
    f = gtpsa._gtpsa._powers_from_type_and_dict
    d = dict(x=2, y=3)
    d2 = f(d, px=5)
    assert d2["px"] == 5
    assert d2["x"] == 2
    assert d2["y"] == 3


def test_powers_dict_illegal_arguments():
    """test if one keyword is a string"""
    f = gtpsa._gtpsa._powers_from_type_and_dict
    d = dict(x=2, y=3)
    with pytest.raises(RuntimeError):
        f(d, x=5.0)


def test_powers_dict_argument_overlapping():
    """argument provided by dict and keyword"""
    f = gtpsa._gtpsa._powers_from_type_and_dict
    d = dict(x=2, y=3)
    d2 = f(d, x=5)
    assert d2["x"] == 2
    assert d2["y"] == 3

def test_get_with_kwarg():

    desc = gtpsa.desc(6, 1)
    t = gtpsa.tpsa(desc, 1)
    t.set_variable(42, "x")
    t.get(x=1)
    assert t.get() == pytest.approx(42)
    assert t.length() == 7

    v = t.getv()

    mapping = gtpsa.default_mapping()
    v = v.copy()
    assert v[0] == 42
    v[0] = 0
    assert v[mapping.x + 1] == pytest.approx(1)
    v[mapping.x + 1] = 0
    assert(np.absolute(v)) == pytest.approx(0, abs=1e-12)


def test_get_with_kwarg_knobs():
    """Check that test knob works as expected
    """
    mapping = named_index
    t = gtpsa.tpsa(desc, 1, mapping=mapping)
    t.set_knob(42, "K")

    # just first order derivatives
    # check that the one required is set
    v = t.getv(1)
    assert v[mapping.K] == pytest.approx(1)
    v[mapping.K] = 0

    # check that the access by key gives the same
    for key, idx in named_index_d.items():
        assert getattr(mapping, key) == idx
        if key == "K":
            assert t.get({key: 1}) == pytest.approx(1)
            assert t.get(K=1) == pytest.approx(1)
        else:
            assert t.get({key:1}) == pytest.approx(0, abs=1e-12)
    assert np.sum(np.absolute([v])) == pytest.approx(0, abs=1e-12)


def test_set_with_kwarg():
    desc = gtpsa.desc(6, 2)
    t = gtpsa.tpsa(desc, 2)
    t.set(constant=3, x=2)


def test_add():
    a = 3
    b = 5
    desc = gtpsa.desc(6, 2)
    t = gtpsa.tpsa(desc, 2)
    t2 = gtpsa.tpsa(desc, 2)

    t.set(0, a)
    assert t.get() == pytest.approx(a, 1e-12)
    t2.set(0, b)
    assert t2.get() == pytest.approx(b, 1e-12)

    t3 = t + t2
    assert t3.get() == pytest.approx(a + b, 1e-12)


def test_basis_operation():
    a = 3
    b = 5
    desc = gtpsa.desc(6, 2)
    t = gtpsa.tpsa(desc, 2)
    t2 = gtpsa.tpsa(desc, 2)

    t.set(0, a)
    assert t.get() == pytest.approx(a, 1e-12)
    t2.set(0, b)
    assert t2.get() == pytest.approx(b, 1e-12)

    t3 = t + t2
    assert t3.get() == pytest.approx(a + b, 1e-12)

    sample = t - t2
    assert sample.get() == pytest.approx(a - b, 1e-12)

    sample = t * t2
    assert sample.get() == pytest.approx(a * b, 1e-12)

    sample = t / t2
    assert sample.get() == pytest.approx(a / b, 1e-12)
    sample.get(x=2)

    sample = t2.copy()
    assert sample.get() == pytest.approx(b, 1e-12)

    sample += t
    assert sample.get() == pytest.approx(b + a, 1e-12)

    sample = t.copy()
    sample -= t2
    assert sample.get() == pytest.approx(a - b, 1e-12)

    sample = t.copy()
    sample *= t2
    assert sample.get() == pytest.approx(a * b, 1e-12)

    sample = t.copy()
    sample /= t2
    assert sample.get() == pytest.approx(a / b, 1e-12)

    sample = t.copy()
    sample += b
    assert sample.get() == pytest.approx(a + b, 1e-12)

    sample = t.copy()
    sample -= b
    assert sample.get() == pytest.approx(a - b, 1e-12)

    sample = t.copy()
    sample *= b
    assert sample.get() == pytest.approx(a * b, 1e-12)

    sample = t.copy()
    sample /= b
    assert sample.get() == pytest.approx(a / b, 1e-12)

    sample = t + b
    assert sample.get() == pytest.approx(a + b, 1e-12)
    sample.get(x=1)

    sample = t - b
    assert sample.get() == pytest.approx(a - b, 1e-12)
    sample.get(px=1)

    sample = t * b
    assert sample.get() == pytest.approx(a * b, 1e-12)
    sample.get(y=1)

    sample = t / b
    assert sample.get() == pytest.approx(a / b, 1e-12)
    sample.get(py=1)


def test_knob_multiplication():
    """just a simple cross check
    """
    mapping = named_index
    k_val = 1.2
    x_val = 2e-3
    y_val = 3e-4
    x = gtpsa.tpsa(desc, 2, mapping=mapping)
    x.set_variable(x_val, "x")

    c2 = gtpsa.tpsa(desc, 2, mapping=mapping)
    c2.set_knob(k_val, "K")

    t = c2 * x
    assert t.get() == pytest.approx(k_val * x_val)
    assert t.get(x=1) == pytest.approx(k_val)
    assert t.get(K=1) == pytest.approx(x_val)
    assert t.get(x=1, K=1) == pytest.approx(1)
    assert t.get(x=2) == pytest.approx(0, abs=1e-12)
    assert t.get(K=2) == pytest.approx(0, abs=1e-12)

    y = gtpsa.tpsa(desc, 2, mapping=mapping)
    y.set_variable(y_val, "y")

    t = c2 * y
    assert t.get() == pytest.approx(k_val * y_val)
    assert t.get(y=1) == pytest.approx(k_val)
    assert t.get(K=1) == pytest.approx(y_val)
    assert t.get(y=1, K=1) == pytest.approx(1)
    assert t.get(y=2) == pytest.approx(0, abs=1e-12)
    assert t.get(K=2) == pytest.approx(0, abs=1e-12)

if __name__ == "__main__":
    test_set_knob_as_var()
    test_get_with_kwarg()