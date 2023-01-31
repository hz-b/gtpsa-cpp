import gtpsa

desc = gtpsa.desc(6, 2)
ct = gtpsa.ctpsa(desc, 2)

ct.set0(0j, 2 + 3j)
ct.name = "complex"

real_part = ct.real()
imag_part = ct.imag()
real_part.name = "real part"
imag_part.name = "imaginary part"

ct.print("z = x + i* y")
real_part.print("x")
imag_part.print("y")

rp = ct.polar()
rp.name = "polar: r, phi"
print(rp.name)
rp.print("polar: radius, angle")

ut = ct.unit()
ut.name = "unit = ct / abs(ct)"
ut.print("(z) / abs(z)")
