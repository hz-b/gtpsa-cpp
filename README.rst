C++ Interface to Truncated Power Series Algebra Module Interface
=================================================================

This code base is providing a shallow c++ wrapper to the
truncatad power series algebra module as provided in mad-ng

	https://github.com/MethodicalAcceleratorDesign/MAD.

For details see

	https://github.com/MethodicalAcceleratorDesign/MAD/blob/dev/src/libgtpsa/README.GTPSA.

	http://mad.web.cern.ch/mad/releases/madng/html/index.html

	http://mad.web.cern.ch/mad/releases/madng/html/mad_mod_diffalg.html


**NB**: this code base modifies the C function defintions of the original code.
For serious work please checkout the original code, in particular if you are using the "C" language.

C++ and Python Pybind11 Interfaces
==================================

The Python Pybind11 <- C++ <- C *gtpsa* interface was prototyped and implemented by Pierre Schnizer.

References:
	P\. Schnizer, W. Khail, J. Bengtsson *Small Talk on AT* IPAC 2022

	https://accelconf.web.cern.ch/ipac2022/papers/tupost029.pdf

	L\. Deniau, C. Tomoiagă *Generalised Truncated Power Series Algebra for Fast Particle Accelerator Transport Maps* IPAC 2015

	https://accelconf.web.cern.ch/ipac2015/papers/mopje039.pdf

Turned out that the gtpsa map concatenator can not handle parameter dependence; so it had to be reimplemented.

The C++ <- C gtpsa bridge interface is in:

	../src/gtpsa/python/src/gtpsa.cc

	/*
	 * Implementation split up in three parts:
	 *
	 * 1. bridge: defined as template
	 * 2. operator functions using the bridge
	 * 3. class using c++ operators defined in a template
	 * 4. class providing full functionality derived from the template
	 *
	 * This splits the functionality in different parts. Hopefully that
	 * makes the code a little more maintainable
	 */


However, some of the key *gtpsa* map analysis functions are implemented in the *Lua* scripting language; see below.

Hence, they have been re-implemented in C++.

	num_t double
	ord_t unsigned char
	idx_t int32_t
	ssz_t int32_t


C++ -> Python Pybind11 Part
---------------------------
The *gtpsa* Python Pybind11 <- C++ part is in:

	../python/src/thor_scsi.cc

		inv
		pinv
		compose
		M_to_h_DF

	../src/gtpsa/python/src/gtpsa.cc

	../src/gtpsa/python/src/ss_vect.cc

	TPSA map operations:

		minv
		pminv
		compose (redundant)
		...

	../src/gtpsa/python/src/gtpsa.cc

		print
		(Sets *eps* 1e-30 vs. 0 for the *gtpsa* print function to supress printing ofzeroes)
		length
		getDescription
		get
		set
		getv
		setv

		...

The *gtpsa* C++ <- C functions are in:

	../src/gtpsa/c++/gtpsa/bridge/bridge.hpp

		mono
		index

		get
		set

		getv
		setv

		getsm
		setVariable (Set monomial: e.g. setVariable(a, 0e0, 2, 0e0)
		rgetorder
		print
		cst
		pow
		add
		dif
		sub
		mul
		div
		acc
		scl
		inv
		invsqrt
		rderiv
		rinteg
		...

	../src/gtpsa/c++/gtpsa/mad/wrapper.tpp

		print()
		print("", 1e-30, 0, stdout) (For TPSA vector; use cout << for map)

		rgetOrder

		mono

		get0(void)
		geti(const idx_t i)
		gets(const std::string s)
		getm(const std::vector<ord_t> &m)
		getv(const idx_t i, std::vector<num_t> *v)

		setvar (Set monomial)

		a*x[0]+b
		set0(const num_t a, const num_t b)

		a*x[i]+b
		seti(const idx_t i, const num_t a, const num_t b)

		a*x[m]+b
		sets(const std::string &s, const num_t a, const num_t b)

		a*x[m]+b
		setm(const std::vector<ord_t> &m, const num_t a, const num_t b)

		setv(const idx_t i, const std::vector<num_t> &v)

		rderiv
		rinteg

	../src/gtpsa/c++/gtpsa/mad/tpsa_wrapper.hpp
	Wrapper for C++ <- C.

		norm
		equ

	../src/gtpsa/c++/gtpsa/bridge/container.hpp

		size
		getMaximumOrder
		computeNorm
		rvec2fld

		...

	../src/gtpsa/c++/gtpsa/mad/container_wrapper.tpp

		size
		getMaximumOrder
		computeNorm
		rvec2fld
		fld2vec
		fgrad
		rliebra
		rexppb
		rlogpb
		rcompose (which call compose in the gtpsa library)
		rminv
		rpminv

	../src/gtpsa/c++/gtpsa/intern/with_operators.hpp

		show()
		show(stdout, level) (For TPSA vector)
		operator<<

		Remark: It only prints leading order; *level* parameter not implemented.

The *gtpsa* print functions are in:

	../src/gtpsa/mad-ng/src]/mad_tpsa_io.c

	../src/gtpsa/mad-ng/src]/mad_tpsa_comp.c

		print
		print_damap

*Gtpsa* C++ <- C Interface
--------------------------
The general *gtpsa* C++ <- C interface is in:

	../src/gtpsa/c++/gtpsa/desc.hpp

	../src/gtpsa/c++/gtpsa/desc.cc

		show
		getDescription
		getinfo
		getNumberOfVariables
		getVariablesMaximumOrder
		getNumberOfParameters
		getParametersMaximumOrder
		getTotalNumber
		getOrderPerParameter
		getNv(ord_t *mo_=0, int *np_=0, ord_t *po_=0)
		maxOrd(int nn=0, ord_t *no=nullptr)
		maxLen(ord_t mo)
		trunc(const ord_t to) (From mad_desc_gtrunc)

	../src/gtpsa/c++/gtpsa/ss_vect.h

	../src/gtpsa/c++/gtpsa/ss_vect.cc

		ss_vect_n_dim
		ss_vect
		state_space
		show(std::ostream &strm, int level = 1, bool with_endl = true) (For TPSA map)

		jacobian
		hessian
		set_zero
		set_identity
		setConstant
		setJacobian
		setHessian
		rcompose

	../src/gtpsa/c++/gtpsa/funcs.h

		sqrt
		exp
		log
		...


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
		compose
		M_to_h_DF

TPSA descriptor operations:

	../src/gtpsa/mad-ng/src/mad_desc.h

	../src/gtpsa/mad-ng/src/mad_desc.c

TPSA vector operations:

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

TPSA map operations:

	../src/gtpsa/mad-ng/src/mad_tpsa_comp.c

		Local

		print_damap

		Public

		compose
		translate
		eval


	../src/gtpsa/mad-ng/src]/mad_tpsa_comp_s.tc

		compose

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
		mnrm (norm)

Also, a few are in:

(coded in *Lua*)

	../src/gtpsa/mad-ng/src/madl_damap.mad

		map_ctor
		factor_map

		Factored Lie of exponential and poisson bracket:

			r = exp(:y1:) exp(:y2:)... x

		lieexppb
		flofacg
		...

	../src/gtpsa/madl_gphys.mad

		make_symp (Make map symplectic, thesis by Liam Healy)

			L\. Healy *Lie-Algebraic Methods for Treating Lattice Parameter Errors in Particle Accelerators* Thesis, Univ. of Maryland, 1986.

		gphys.normal_ng (Map normal form)
		normal_c        (Phasor basis)

*Lua* Scripting Language
----------------------
The *Lua* scripting language (Portuguese: *lua* -> *moon*) was created by the Computer Graphics
Technology Group (Tecgraf) at the PUC Univ., Rio de Janeiro, Brazil in 1993:

	https://www.lua.org/about.html

LuaJiT is a just-in-time compiler:

	https://luajit.org/luajit.html
