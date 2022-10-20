from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os.path

t_dir = os.path.dirname(__file__)
gtpsa_inc_dir = os.path.join(t_dir, os.pardir, "c++")
mad_inc_dir = os.path.join(t_dir, os.pardir, "mad-ng", "src")

prefix = os.path.abspath(os.path.join(os.environ["HOME"], ".local"))
inc_dir = os.path.join(prefix, "include")
lib_dir = os.path.join(prefix, "lib")

ext_modules = [
    Pybind11Extension(
        "lib_gtpsa",
        sorted(["src/gtpsa.cc"]),
        include_dirs=[inc_dir],
        libraries=["gtpsa"],
        library_dirs=[lib_dir],
    )
]

setup(cmdclass={"build_ext": build_ext}, ext_package="gtpsa", ext_modules=ext_modules)
