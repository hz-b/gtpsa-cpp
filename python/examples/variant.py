"""Using an object that can be a double or tpsa object

"""
import gtpsa

# create the object .. defaults to double
tmp = gtpsa.TpsaOrDouble();
# add to it for setting a constant value
tmp += 42e0
print("tmp", tmp)
tmp = gtpsa.TpsaOrDouble();
# add to it for setting a constant value
tmp += 42e0
print("tmp", tmp)
d = tmp.to_object()
print("tmp to python object", d, type(d))

# Create the object with a tpsa object under the hood
print("tpsa constructed")
desc = gtpsa.desc(6,2)
t = gtpsa.tpsa(desc, 1)

tmp = gtpsa.TpsaOrDouble(t);
print("tpsa or double:", tmp, type(tmp))
# make it a "pure python object"
d = tmp.to_object()
print(d, type(d))
