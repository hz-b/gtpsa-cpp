C++ Interface to Truncated Power Series Algebra Module Interface
=================================================================

This code base is providing a shallow c++ wrapper to the
truncatad power series algebra module as provided in mad-ng
	https://github.com/MethodicalAcceleratorDesign/MAD.

For details see
	https://github.com/MethodicalAcceleratorDesign/MAD/blob/dev/src/libgtpsa/README.GTPSA.


**NB**: this code base modifies the C function defintions of the original code.
For serious work please checkout the original code, in particular if you are using the "C" language.

C++ and Python Pybind11 Interfaces
==================================

The *gtpsa* C -> C++ -> Python Pybind11 interfaces were implemented by Pierre Schnizer.

References:
	P\. Schnizer, W. Khail, J. Bengtsson *Small Talk on AT* IPAC 2022

	https://accelconf.web.cern.ch/ipac2022/papers/tupost029.pdf

	L\. Deniau, C. Tomoiagă *Generalised Truncated Power Series Algebra for Fast Particle Accelerator Transport Maps* IPAC 2015

	https://accelconf.web.cern.ch/ipac2015/papers/mopje039.pdf

However, some of the key *gtpsa* map analysis functions are implemented in the Lua scripting language; see below.

Hence, they have been re-implemented in C++.

C++ -> Python Pybind11 Part
---------------------------
The *gtpsa* C++ -> Python Pybind11 part is in:

	../src/gtpsa/python/src/gtpsa.cc

	../src/gtpsa/python/src/ss_vect.cc

which also sets eps for the *gtpsa* print function; see below.
(Set to e.g. 1e-30 vs. 0 to supress printing of zeroes)

The *gtpsa* I/O C -> C++ functions are in:

	../src/gtpsa/c++/gtpsa/mad/wrapper.tpp

		print()

		print("", 1e-30, 0, stdout)

		rgetOrder

		rderiv

		rinteg

	../src/gtpsa/c++/gtpsa/mad/tpsa_wrapper.hpp

		norm

		equ

	../src/gtpsa/c++/gtpsa/mad/container_wrapper.tpp

		getMaximumOrder

		computeNorm

		rvec2fld

		fld2vec

		fgrad

		rliebra

		rexppb

		rlogpb

		rcompose

		rminv

		rpminv

and in:

	../src/gtpsa/c++/gtpsa/intern/with_operators.hpp

		show()

		show(stdout, level)

		Remark: It only prints leading order; *level* parameter not implemented.

The *gtpsa* print functions are in:

	../src/gtpsa/mad-ng/src]/mad_tpsa_io.c

	../src/gtpsa/mad-ng/src]/mad_tpsa_comp.c

		print

		print_damap

*gtpsa* C -> C++ Interface
------------------------
The general *gtpsa* C -> C++ interface is in:

	../src/gtpsa/c++/gtpsa/ss_vect.h

	../src/gtpsa/c++/gtpsa/ss_vect.cc

		show(std::ostream &strm, int level = 1, bool with_endl = true)

		jacobian

		hessian

		set_zero

		set_identity

		setConstant

		setJacobian

		setHessian


Not yet implemented:

(For TPSA maps)

	rminv

	rpminv

	rcompose

	rvec2fld

	fld2vec

	fgrad

	rliebra

	rexppb

	rlogpb

	rderiv

	../src/gtpsa/c++/gtpsa/lielib.cc

		inv

		pinv

		M_to_h_DF

TPSA vector operations are in:

	../src/gtpsa/mad-ng/src/mad_tpsa.h

	../src/gtpsa/mad-ng/src/mad_tpsa_ops.c

		add

		sub

		...

		integ

		deriv

		poisbra

		...

		print

		...

		cutord

TPSA map operations are in:

	../src/gtpsa/mad-ng/src/mad_tpsa_comp.c

		Local

		print_damap

		Public

		compose

		translate

		eval

	../src/gtpsa/mad-ng/src]/mad_tpsa_minv.c

		minv

		pinv

	../src/gtpsa/mad-ng/src/mad_tpsa_mops.c

		Local

		print_damap

		Public

		exppb

		logpb

		liebra

		fgrad

		Compute (Eq. (34)):

			G(x;0) = -J grad.f(x;0)
		vec2fld


		Compute(Eqs. (34)-(37)):

			f(x;0) = \int_0^x J G(x';0) dx' = x^t J phi G(x;0)

		fld2vec

		mnrm

Also, a few are in:

(coded in LUA)

	../src/gtpsa/mad-ng/src/madl_damap.mad

		map_ctor

		factor_map

		Factored Lie of exponential and poisson bracket:

			r = exp(:y1:) exp(:y2:)... x

		lieexppb

		flofacg

		...

	../src/gtpsa/madl_gphys.mad

		make_symp (Make map symplectic, thesis by Liam Healey)

		gphys.normal_ng (Map normal form)

		normal_c(Phasor basis)

Lua Scripting Language
----------------------
The Lua scripting language (Portuguese: lua -> moon) was created by the Computer Graphics
Technology Group (Tecgraf) at the PUC Uni, Rio de Janeiro, Brazil in 1993:

	https://www.lua.org/about.html

LuaJiT is a just-in-time compiler:

	https://luajit.org/luajit.html
