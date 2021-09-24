# Blob.py
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
from .Collections import *
from .common import *

class Blob (object):
    def __init__(self, data, *, contentType =None, fdict =None):
        if fdict != None:
            super.__init__(lib.CBLBlob_Get(fdict), "Dict is not a Blob")
        else:
            super.__init__(lib.CBLBlob_CreateWithData(contentType, asSlice(data)),
                           "Failed to create Blob")

    @property
    def digest(self):
        return pystr(lib.CBLBlob_Digest(self._ref))

    @property
    def length(self):
        return lib.CBLBlob_Length(self._ref)

    @property
    def contentType(self):
        return pystr(lib.CBLBlob_ContentType(self._ref))
    
    @property
    def data(self):
        if "_data" in self.__dict__:
            return self._data
        elif self.digest != None:
            sliceResult = lib.CBLBlob_LoadContent(self._ref, gError)
            # OPT: This copies the bytes
            result = sliceResultToBytes(sliceResult)
            lib.FLSliceResult_Release(sliceResult)
            return result
        else:
            return None

    def __repr__(self):
        r = "Blob["
        if self.contentType != None:
            r += self.contentType
        if self.length != None:
            if self.contentType != None:
                r += ", "
                r += self.length + " bytes"
        return r + "]"

    def _jsonEncodable(self):
        return decodeFleeceDict( lib.CBLBlob_Properties(self._ref), depth=99 )
