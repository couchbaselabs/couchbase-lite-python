#! /usr/bin/env python3
#
# build.py
#
# Copyright (c) 2019-2021 Couchbase, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# NOTE: This script should be run in the `CouchbaseLite` subdirectory.

import argparse
import os.path
import os
from pathlib import Path
import platform
import shutil
import subprocess
from cffi import FFI    # `pip3 install cffi` to install this


# TODO: Make the stuff below work with Windows

DEFAULT_LIBRARIES    = "cblite z"
DEFAULT_LINK_ARGS    = ""

ffibuilder = FFI()

if platform.system() == "Darwin":
    DEFAULT_INCLUDE_DIR  = "/usr/local/include"
    DEFAULT_LIBRARY_PATH = "/usr/local/lib/libcblite.3.dylib"
elif platform.system() == "win32":
    print("FOO")
else:
    DEFAULT_INCLUDE_DIR  = "/usr/include"
    triplet = subprocess.check_output("gcc -print-multiarch", shell=True).decode('ascii').strip()
    DEFAULT_LIBRARY_PATH = f"/usr/lib/{triplet}/libcblite.so.3"


def BuildLibrary(includeDir, python_includedir, lib_path, libraries, extra_link_args, buildEE, verbose):
    include_dirs = [includeDir]
    if python_includedir:
        # when cross-compiling, use python headers for target rather than build system
        include_dirs.insert(0, python_includedir)

    # python 3.5 distutils breaks on Linux with absolute library path,
    # so make sure it is relative path
    lib_path = os.path.relpath(lib_path)
    shutil.copy(lib_path, "..")

    # CFFI stuff -- see https://cffi.readthedocs.io/en/latest/index.html

    # This is passed to the real C compiler and should include the declarations of
    # the symbols declared in cdef()
    cHeaderSource = r"""#include <cbl/CouchbaseLite.h>"""

    ffibuilder.cdef(CDeclarations(buildEE))
    ffibuilder.set_source(
        "_PyCBL",       # Module name
        cHeaderSource,
        libraries=libraries,
        include_dirs=include_dirs,
        library_dirs=[lib_path],
        extra_link_args=extra_link_args)


def CDeclarations(buildEE):
    f = open("../CBLForPython.h", "rb", buffering=0)
    result = str(f.readall(), encoding="utf-8")
    if buildEE:
        f = open("../CBLForPython_EE.h", "rb", buffering=0)
        result += str(f.readall(), encoding="utf-8")
    return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="build Couchbase Lite Python bindings")
    parser.add_argument('--edition', 
                        choices=['CE', 'EE'], 
                        default='CE',
                        help="Couchbase Lite edition. Defaults to CE")
    parser.add_argument('--include', '-I', 
                        default=DEFAULT_INCLUDE_DIR, 
                        metavar="PATH",
                        help="C include path containing `cbl/` subdir. Defaults to " + DEFAULT_INCLUDE_DIR)
    parser.add_argument('--library', 
                        default=DEFAULT_LIBRARY_PATH, 
                        metavar="PATH",
                        help="Path to Couchbase Lite C library. Defaults to " + DEFAULT_LIBRARY_PATH)
    parser.add_argument('--link', 
                        default=DEFAULT_LIBRARIES, 
                        metavar="LIBS",
                        help="C library names to link. Defaults to '" + DEFAULT_LIBRARIES + "'")
    parser.add_argument('--link_flags', 
                        default=DEFAULT_LINK_ARGS, 
                        metavar="FLAGS",
                        help="Linker flags. Defaults to '" + DEFAULT_LINK_ARGS + "'")
    parser.add_argument('--python_includedir', 
                        default='', 
                        metavar="PATH",
                        help="Directory containing python headers (specify for cross-compiling)")
    parser.add_argument('--verbose', 
                        action='store_true',
                        help="Show verbose output from CFFI")
    args = parser.parse_args()

    linkFlags = None
    if args.link_flags != None:
        linkFlags = args.link_flags.split()

    buildEE = (args.edition == 'EE')

    print(f"Building Python bindings with headers from {args.include}/cbl and library {args.library}")

    BuildLibrary(args.include, args.python_includedir, args.library, args.link.split(), linkFlags, buildEE, args.verbose)
    ffibuilder.compile(verbose=args.verbose)

    os.remove("_PyCBL.c")
    os.remove("_PyCBL.o")
else: # Built using setuptools (probably)
    last = os.getcwd()
    os.chdir(Path(os.path.dirname(os.path.realpath(__file__)), "CouchbaseLite"))
    BuildLibrary("", None, DEFAULT_LIBRARY_PATH, DEFAULT_LIBRARIES.split(), None, False, False)
    os.chdir(last)