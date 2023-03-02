"""Using tpsa object

Some simple examples
"""
import gtpsa
import numpy as np

# The description object provides number of variables and maximum
# order.Internally it contains look up tables for calculations and
# thus requires to be the same for all (c)tpsa objects used in the
# calculation
desc = gtpsa.desc(6, 2)
t = gtpsa.tpsa(desc, 2);

# Assign a name to the object
t.name = "test"
# Set the variable: i.e. its value and its gradient. Specifying the
# gradient with name is experimental
t.set_variable(42, "x")

# Use gtpsa c function to print. will not always work: in
# particular jupyter notebooks will not grab the output
t.print()

# Coefficients of the tpsa object
# 1. the order of the coefficents
# 2. the coefficient
# 3. the index it is placed (internal info of the tpsa object)
coeffs = t.get_coefficients()
print("coefficients", coeffs)

# Accessing the coefficients
# The constant part
print("constant part", t.get())

# Accessing coefficient for higher orders ... just specifiy the
# powers in a dictionary
print("gradient for x", t.get(dict(x=1)))

# Or define the power as an array
orders = np.zeros(6, int)
orders[0] = 1
print("gradient for x", t.get(orders))
print("curvature for delta", t.get(dict(delta=2)))

# set everyting to zero
t.clear()
# length .. number of maximum coefficients ?
print("length", t.length())

# order of this object
print("order", t.order)

# available methods
print(dir(t))


# Power handled by dedicated c++ function
tmp = t**2
print(tmp, type(tmp))
print("gradient for x", tmp.get(dict(x=1)))
