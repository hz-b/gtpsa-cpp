import pytest

import gtpsa
from gtpsa.utils_df import tpsa2df


def test_export_to_df_6vars():
    """just check that data get exported"""
    desc = gtpsa.desc(6, 2)
    t = gtpsa.tpsa(desc, 1)
    t.set_variable(2, "x")
    df = tpsa2df(t)
    assert df.value.iloc[0] == pytest.approx(2, 1e-12)
    assert float(
        df.value.loc[(df.x == 1) & (df.y == 0) & (df.px == 0) & (df.py == 0)]
    ) == pytest.approx(1, 1e-12)


def test_export_to_df_1var():
    """just check that data get exported"""
    d = dict(x=0)
    mapping = gtpsa.IndexMapping(d, check_default_keys=False)
    desc = gtpsa.desc(1, 2)
    t = gtpsa.tpsa(desc, 1, mapping)
    t.set_variable(2, "x")
    df = tpsa2df(t)
    assert df.value.iloc[0] == pytest.approx(2, 1e-12)
    assert float(df.value.loc[(df.x == 1)]) == pytest.approx(1, 1e-12)


def test_export_to_df_6vars_3knobs():
    """just check that data get exported"""
    d = dict(x=0, y=2, px=1, py=3, delta=4, ct=5, dk=6, dl=7, dm=8)
    mapping = gtpsa.IndexMapping(d)
    desc = gtpsa.desc(6, 2, 3, 1)
    t = gtpsa.tpsa(desc, 1, mapping)
    t.set_variable(2, "x")
    df = tpsa2df(t)
    assert df.value.iloc[0] == pytest.approx(2, 1e-12)
    assert float(df.value.loc[(df.x == 1)]) == pytest.approx(1, 1e-12)
