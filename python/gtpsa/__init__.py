"""General truncated power series

This wrapper covers
"""
from ._gtpsa import *

# Formerly wrapped in C++ ...
# now much simpler code in python
# del tpsa, ctpsa, ss_vect_double, ss_vect_tpsa

# a better name for wrapper
# from .gtpsa_delegator import tpsa, ctpsa
# from .phase_space_delegator import ss_vect_tpsa, ss_vect_double

__all__ = [
    "acos",
    "acos_",
    "acosh",
    "acosh_",
    "acot",
    "acot_",
    "acoth",
    "acoth_",
    "asin",
    "asin_",
    "asinc",
    "asinc_",
    "asinh",
    "asinh_",
    "asinhc",
    "asinhc_",
    "atan",
    "atan_",
    "atanh",
    "atanh_",
    "compose",
    "cos",
    "cos_",
    "cosh",
    "cosh_",
    "cot",
    "cot_",
    "coth",
    "coth_",
    "desc",
    "erf",
    "erf_",
    "erfc",
    "erfc_",
    "exp",
    "exp_",
    "init",
    "log",
    "log_",
    "sin",
    "sin_",
    "sinc",
    "sinc_",
    "sinh",
    "sinh_",
    "sinhc",
    "sinhc_",
    "sqrt",
    "sqrt_",
    "tan",
    "tan_",
    "tanh",
    "tanh_",

    "tpsa",
    "ctpsa",

    "ss_vect_tpsa",
    "ss_vect_double",

    "utils",

    "IndexMapping",

    "default_mapping"
]
