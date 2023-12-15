import pytest
import gtpsa


def test_default_mapping_get_index():
    mapping = gtpsa.default_mapping()
    assert mapping.index("x") == 0
    assert mapping.index("y") == 2

    # the swapped dimensions
    assert mapping.index("ct") == 5


def test_default_mapping_by_attribute():
    index_map = gtpsa.default_mapping()
    assert index_map.x == 0
    assert index_map.px == 1
    assert index_map.y == 2
    assert index_map.py == 3
    assert index_map.delta == 4
    assert index_map.ct == 5


def test_mapping_with_knobs():

    d = dict(x=0, y=2, px=1, py=3, delta=4, ct=5, K=6, dx=7, dy=8)
    mapping = gtpsa.IndexMapping(d)
    assert mapping.index("x") == 0
    assert mapping.index("y") == 2
    # the swapped dimensions
    assert mapping.index("delta") == 4
    assert mapping.index("K") == 6
    assert mapping.index("dy") == 8

    assert mapping.dx == 7
    assert mapping.py == 3