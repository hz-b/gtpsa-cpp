from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os.path
import logging

logger = logging.getLogger("gtpsa-build")

from pybind11.setup_helpers import ParallelCompile

# Optional multithreaded build
try:
    os.environ["NPY_NUM_BUILD_JOBS"]
except KeyError:
    os.environ["NPY_NUM_BUILD_JOBS"] = "5"

ParallelCompile("NPY_NUM_BUILD_JOBS").install()


# still needed ?
t_dir = os.path.dirname(__file__)
gtpsa_inc_dir = os.path.join(t_dir, os.pardir, "c++")
mad_inc_dir = os.path.join(t_dir, os.pardir, "mad-ng", "src")

# How to define where the thor scsi library is located?
# Some hard coded preferences for your convienience
# prefix = os.path.abspath(os.path.join(os.environ["HOME"], ".local"))
# prefix = os.path.abspath(os.path.join(os.environ["HOME"], "git", "ts-lib-dev",
#                                      "local"))

# deliberatley chosen _PREFIX instead of DIR ...
# not to interfer with cmake's variables
prefix = None
try:
    prefix = os.environ["gtpsa_PREFIX"]
except KeyError as ke:
    logger.info(f"no environment variable gtpsa_PREFIX: {ke}")

include_dirs = []
library_dirs = []
if prefix:
    include_dirs += [os.path.join(prefix, "include")]
    library_dirs += [os.path.join(prefix, "lib")]


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
        define_macros=[("GTPSA_DEFINE_BOOL",1)],
        include_dirs=include_dirs + ["/usr/local/include"],
        libraries=["gtpsa", "gtpsa-c++", "armadillo"],
        library_dirs=library_dirs + ["/usr/local/lib"],
    )
]

setup(cmdclass={"build_ext": build_ext}, ext_package="gtpsa", ext_modules=ext_modules)
