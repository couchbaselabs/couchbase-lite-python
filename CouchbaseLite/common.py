# common.py
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


from ._PyCBL import ffi, lib

# def cstr(str):
#     """Copies a Python string to a heap-allocated C string."""
#     return ffi.new("char[]", str.encode("utf-8"))

# def pystr(cstr):
#     """Copies a C string to a Python string."""
#     return str(ffi.string(cstr), "utf-8")

def sliceToString(s):
    """Copies a FLSlice to a Python string."""
    if s.buf == None:
        return None
    return str(ffi.string(ffi.cast("const char*", s.buf), s.size), "utf-8")

def sliceResultToString(sr):
    """Copies a FLSliceResult to a Python string and frees it."""
    str = sliceToString(sr)
    lib.FLSliceResult_Release(sr)
    return str

def sliceResultToBytes(sr):
    """Copies a FLSliceResult to a Python bytes object. Does not free the FLSliceResult."""
    if sr.buf == None:
        return None
    lib.FLSliceResult_Release(sr)
    b = bytes( ffi.buffer(sr.buf, sr.size) )
    return b

def asSlice(data):
    """Returns a FLSlice pointing to the data."""
    buffer = ffi.from_buffer(data)
    s = ffi.new("FLSlice")
    s.buf = buffer
    s.size = len(buffer)
    return s

def stringParam(str):
    """Returns a pointer/length array suitable for passing to an FLSlice C function parameter."""
    utf8 = str.encode()
    buffer = ffi.from_buffer(utf8)
    return [buffer, len(buffer)]

# A global CBLError object to use in API calls, so each call doesn't have to
# allocate a new one. (This is fine as long as we're single-threaded.)
gError = ffi.new("CBLError*")


class CBLException (EnvironmentError):
    def __init__(self, message, cblError = None):
        if cblError != None:
            self.domain = cblError.domain
            self.code = cblError.code
            self.error = sliceResultToString(lib.CBLError_Message(cblError))
            EnvironmentError.__init__(self, message + ": " + self.error)
        else:
            EnvironmentError.__init__(self, message)


class CBLObject (object):
    def __init__(self, ref, message =None, error =None):
        self._ref = ref
        if not ref and message:
            raise CBLException(message, error)

    def __del__(self):
        if lib != None and "_ref" in self.__dict__ and self._ref != None:
            lib.CBL_Release(self._ref)


class ListenerToken (object):
    def __init__(self, owner, handle, c_token):
        self.owner = owner
        self.handle = handle
        self.c_token = c_token

    def __del__(self):
        self.remove()

    def remove(self):
        if self.owner != None:
            lib.CBLListener_Remove(self.c_token)
            self.owner.listeners.remove(self.handle)
            self.owner = None
            self.handle = None
