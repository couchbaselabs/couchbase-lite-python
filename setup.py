from ensurepip import version
from setuptools import setup, find_packages, Extension

setup(
    name="cblite",
    version="0.1.0",
    description="Python bindings for Couchbase Lite",
    url="https://github.com/couchbaselabs/couchbase-lite-python",
    author="Couchbase, Inc.",
    author_email="foo@couchbase.com",
    license="Apache 2.0",
    packages=find_packages(),
    setup_requires=["cffi>=1.0.0"],
    cffi_modules=[
        "build.py:ffibuilder"
    ],
    install_requires=["cffi>=1.0"],
    ext_modules=[Extension("libcblite", sources=["CouchbaseLite/empty.c"], libraries=["cblite"])]
)