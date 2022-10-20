import gtpsa
import numpy as np

desc = gtpsa.desc(6, 4)
t1 = gtpsa.tpsa(desc, gtpsa.init.default)
t2 = gtpsa.tpsa(t1.getDescription(), gtpsa.init.default)

nums = [np.pi / 6] + [1] * 6
t1.setv(0, nums)
print(t1)

t2 = gtpsa.sin(t1)
t3 = gtpsa.asin(t2)
print(t2)
print(t3)
