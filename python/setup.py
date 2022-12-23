from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os.path

from pybind11.setup_helpers import ParallelCompile

# Optional multithreaded build
try:
    os.environ["NPY_NUM_BUILD_JOBS"]
except KeyError:
    os.environ["NPY_NUM_BUILD_JOBS"] = "5"

ParallelCompile("NPY_NUM_BUILD_JOBS").install()

t_dir = os.path.dirname(__file__)
gtpsa_inc_dir = os.path.join(t_dir, os.pardir, "c++")
mad_inc_dir = os.path.join(t_dir, os.pardir, "mad-ng", "src")

prefix = os.path.abspath(os.path.join(os.environ["HOME"], ".local"))
#prefix = os.path.abspath(os.path.join(os.environ["HOME"], "git", "ts-lib-dev",
#                                      "local"))
inc_dir = os.path.join(prefix, "include")
lib_dir = os.path.join(prefix, "lib")

ext_modules = [
    Pybind11Extension(
        "_gtpsa",
        sorted([
            "src/gtpsa_module.cc",
            "src/desc.cc",
            "src/gtpsa.cc",
            "src/ss_vect.cc",
            "src/arma.cc"
        ]),
        include_dirs=[inc_dir, "/usr/local/include"],
        libraries=["gtpsa", "gtpsa-c++", "armadillo"],
        library_dirs=[lib_dir, "/usr/local/lib"],
    )
]

setup(cmdclass={"build_ext": build_ext}, ext_package="gtpsa", ext_modules=ext_modules)
