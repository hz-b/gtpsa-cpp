"""Use named index
"""
import numpy as np

class NamedIndex:
    def __init__(self, named_index: dict):
        self.named_index = named_index
        self.length = len(list(self.named_index.keys()))

    def index(self, key):
        r = self.named_index[key]
        # Ensure int
        r = int(r)
        return r

    def order_vector_from_power(self, d=None, **kwargs):
        orders = np.zeros(self.length, np.ubyte)
        d.update(kwargs)
        for key, order in d.items():
            index = self.index(key)
            orders[index] = order
        return orders


default_named_index = NamedIndex(dict(x=0, px=1, y=2, py=3, delta=4, ct=5))

__all__ = ["NamedIndex", "default_named_index"]
