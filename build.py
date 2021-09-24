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
import platform
import shutil
from cffi import FFI    # `pip3 install cffi` to install this


# TODO: Make the stuff below work with Windows

DEFAULT_INCLUDE_DIR  = "/usr/local/include"
DEFAULT_LIBRARY_PATH = "/usr/local/lib/libcblite"
DEFAULT_LIBRARIES    = "cblite z"
DEFAULT_LINK_ARGS    = ""

if platform.system() == "Darwin":
    DEFAULT_LIBRARY_PATH += ".dylib"
    DEFAULT_LINK_ARGS = "-rpath @loader_path"  # i.e. look for CBL dylib in same dir as bindings lib
else:
    DEFAULT_LIBRARY_PATH += ".so"


def BuildLibrary(includeDir, python_includedir, lib_path, libraries, extra_link_args, buildEE, verbose):
    include_dirs = [includeDir]
    if python_includedir:
        # when cross-compiling, use python headers for target rather than build system
        include_dirs.insert(0, python_includedir)

    # python 3.5 distutils breaks on Linux with absolute library path,
    # so make sure it is relative path
    lib_path = os.path.relpath(lib_path)

    # Copy the CBL library here:
    shutil.copy(lib_path, ".")

    # CFFI stuff -- see https://cffi.readthedocs.io/en/latest/index.html

    # This is passed to the real C compiler and should include the declarations of
    # the symbols declared in cdef()
    cHeaderSource = r"""#include <cbl/CouchbaseLite.h>"""

    ffibuilder = FFI()
    ffibuilder.cdef(CDeclarations(buildEE))
    ffibuilder.set_source(
        "_PyCBL",       # Module name
        cHeaderSource,
        libraries=libraries,
        include_dirs=include_dirs,
        library_dirs=["."],
        extra_link_args=extra_link_args)
    ffibuilder.compile(verbose=verbose)

    os.remove("_PyCBL.c")
    os.remove("_PyCBL.o")


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

    print("Building Python bindings with headers from ", args.include, "/cbl  and library ", args.library)

    BuildLibrary(args.include, args.python_includedir, args.library, args.link.split(), linkFlags, buildEE, args.verbose)
