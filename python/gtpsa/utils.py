"""Utilities to work with tpsa objects
"""
from typing import Sequence, Tuple, Dict
import numpy as np
from ._gtpsa import tpsa, ctpsa, IndexMapping
import logging

logger = logging.getLogger("gtpsa")


def tpsa_coefficients_to_nrec(
    coeffs: Sequence[Tuple[np.ndarray, float, int]], variable_names: Sequence
) -> np.recarray:
    """
    Args:
        coeffs: coefficients as provided by tpsa
                these are provided

    If you want to use it as a pandas Dataframe just use

        df = pd.Dataframe(r).set_index("index")
    """

    dtype = [("value", float), ("index", int)]
    dtype += [(var_name, int) for var_name in variable_names]

    # need to be tuples ...
    data = [(c[1], c[2]) + tuple(c[0]) for c in coeffs]
    try:
        result = np.array(data, dtype=dtype)
    except ValueError as ev:
        logger.error(
            "Array creation failed: dtype = %s, data = %s variable_names = %s",
            dtype,
            data,
            variable_names,
        )
        raise ev

    result = result.view(np.recarray)
    return result


def mapping_sorted(mapping: IndexMapping) -> Dict:
    """sort entries in a index mapping

    Won't bet that they stay that way
    """
    class SortItem:
        def __init__(self, name):
            self.name = name
            self.index = mapping.index(name)

        def __lt__(self, other):
            return self.index < other.index

        def __gt__(self, other):
            return self.index > other.index

        def __eq__(self, other):
            return self.index == other.index
    tmp = [SortItem(name) for name in dir(mapping)]
    tmp.sort()

    return [o.name for o in tmp]

def tpsa_extract_coefficients_to_nrec(t: tpsa | ctpsa) -> np.recarray:
    """extract coefficents from a tpsa object"""

    coeffs = t.get_coefficients()
    names = mapping_sorted(t.get_mapping())
    return tpsa_coefficients_to_nrec(coeffs, names)


__all__ = ["tpsa_coefficients_to_nrec"]
