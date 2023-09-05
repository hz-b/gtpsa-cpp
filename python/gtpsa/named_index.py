"""Use named index

But check that default index organisation is met

Todo:
   check if `pandas.Index` should be used instead?
"""
import numpy as np


class NamedIndex:
    def __init__(self, named_index: dict):
        self.named_index = named_index
        self.length = len(list(self.named_index.keys()))

    def __dir__(self):
        return list(self.named_index.keys())

    def index(self, key: str) -> int:
        r = self.named_index[key]
        # Ensure int
        r = int(r)
        return r

    def order_vector_from_power(self, d: dict = None, **kwargs):
        #: ubyte as required by mad-ng tpsa package
        orders = np.zeros(self.length, np.ubyte)
        d.update(kwargs)
        for key, order in d.items():
            index = self.index(key)
            orders[index] = order
        return orders


#: should be part of a Hamiltionian tracing package
default_named_index = NamedIndex(dict(x=0, px=1, y=2, py=3, delta=4, ct=5))

__all__ = ["NamedIndex", "default_named_index"]
