import numpy as np
import gtpsa

def test_add():
    desc = gtpsa.desc(nv=6, mo=4)
    print(desc)

    t1 = gtpsa.tpsa(desc, 3)
    del desc

    t1.set(np)
    print(t1)
    t1 += 5
    print(t1)

    t2 = t1 + 3.0
    print(t2)
    print(t2.get())
    print("t2 len", t2.length())
    print("t2 v", t2.getv(0))

if __name__ == '__main__':
    test_add()
