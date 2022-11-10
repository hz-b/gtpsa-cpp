Known BUGS
==========


State space vector
------------------

The space state vector provides `__getitem__` access to the subcomponents.
Depending on its internal type, however, they could return a copy.
Therefore as consequence the following code will not work as
expected (see also state_space_gtpsa.py in the examples directory)


.. ::

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)

    ss1[1].set(0, 2)

This code works as it takes a reference to the returned objects and
returns this object back

.. ::

    desc = gtpsa.desc(6, 2)
    ss1 = gtpsa.ss_vect_tpsa(desc, 1)

    a_tpsa_obj = ss1[1]
    a_tpsa_obj.set(0, 2)
    ss1[1] = a_tpsa_obj

    del a_tpsa_obj
