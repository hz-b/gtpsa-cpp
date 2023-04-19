import pytest
import gtpsa
import numpy as np

nv = 6
mo = 2
n_pars = 3
po = 1

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


@pytest.mark.skip
def test_set_knob_as_var():
    """Test setting a knob using variable interface

    Will raise an error in the tpsa side
    """
    val = 42
    var_index = nv + 2
    t = gtpsa.tpsa(desc, 1)
    t.set_variable(val, var_index, check_first=False)
    print("t\n", t)
    assert t.get() == pytest.approx(val, 1e-12)
    derivs = t.getv(1)
    print("1. derivatives", derivs)
    assert derivs[var_index - 1] == pytest.approx(1, 1e-12)
    derivs[var_index - 1] = 0
    assert np.sum(np.absolute(derivs)) == pytest.approx(0, abs=1e-12)


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
    named_index_d = dict(x=0, px=1, y=2, py=3, delta=4, ct=5, K=6, dx=7, dy=8)
    named_index = gtpsa.IndexMapping(named_index_d)
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
    """test if one keyword is a string
    """
    f = gtpsa._gtpsa._powers_from_type_and_dict
    d = dict(x=2, y=3)
    with pytest.raises(RuntimeError):
        f(d, x=5.0)


def test_powers_dict_argument_overlapping():
    """argument provided by dict and keyword
    """
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


def test_set_with_kwarg():
    desc = gtpsa.desc(6, 2)
    t = gtpsa.tpsa(desc, 2)
    t.set(constant=3, x=2)

if __name__ == "__main__":
    test_set_knob_as_var()
