"""Set constant part of a space state containing tpsa

   getitem returns a new object not one that mangles the
   internal part

   Therefore, you need to assign the returned object to a variable
   change it and then stuff it back into the ss vector

   Warning:
       Do not expect that the returned object will continue to be a new one
       Work in progress
"""
import gtpsa
import numpy as np

desc = gtpsa.desc(6, 2)
ss1 = gtpsa.ss_vect_tpsa(desc, 1)
ss1.px.set(0, 2)

a_tpsa_obj = ss1[1]
a_tpsa_obj.set(0, 2)
ss1[1] = a_tpsa_obj
ss1.px = a_tpsa_obj

print("Entries")
print(dir(ss1))
print(ss1.x)


del a_tpsa_obj

assert (ss1[1].get() == 2)
assert (ss1.cst()[1] == 2)

np.array(ss1)
