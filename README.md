# EXPERIMENTAL Python Bindings For Couchbase Lite

September 2021

This directory contains a Python API binding of [Couchbase Lite For C][CBL_C], based on [CFFI][CFFI].

It's still incomplete, has only had some informal testing, mostly on macOS. It is **NOT SUPPORTED BY COUCHBASE**. Even if you are a Couchbase customer, our support team cannot help you with using this library.

Here's a snippet of code to show what the API looks like:

```python
from CouchbaseLite.Database import Database, DatabaseConfiguration
from CouchbaseLite.Document import MutableDocument

# Open a database:
db = Database("python_db", DatabaseConfiguration("/tmp"));

# Create a document:
doc = MutableDocument("foo")
doc["greeting"] = "Howdy!"
db.saveDocument(doc)

# Read it back:
readDoc = db.getDocument("foo")
props = readDoc.properties
greeting = props["greeting"]
```


## Building

**_"Some assembly required..."_**

### Python and CFFI

You'll need Python 3. Currently we've only tested this with 3.9.

Make sure you have the CFFI package:

    $ pip3 install cffi

### Couchbase Lite For C

Next you need the Couchbase Lite For C shared library and headers. You can download them from Couchbase, or build them yourself using the [Git repo][CBL_C].

### Building CBL-Python

Now you can build the Python binding library:

    $ cd couchbase-lite-python
    $ ./build.sh

This assumes Couchbase Lite has been installed into /usr/local. If not, you'll need to tell the script where to find it:

    $ ./build.sh --include /path/to/include/ --libdir /path/to/libs/

`/path/to/include/` should have a _subdirectory_ named `cbl` with the headers in it. And `/path/to/libs` should contain the `libcblite` library file.

### Try it out

Now try the (rudimentary) tests:

    $ test/test.sh

You can look at the test code in `test/test.py` for examples of how to use the API.

## Learning

If you're not already familiar with Couchbase Lite, you'll want to start by reading through its
[official documentation][CBLDOCS]. The Python API should be mostly compatible with the languages documented there.


[CBL]: https://www.couchbase.com/products/lite
[CBL_C]: https://github.com/couchbaselabs/couchbase-lite-C
[CFFI]: https://cffi.readthedocs.io/en/latest/index.html
[CBLDOCS]: https://docs.couchbase.com/couchbase-lite/current/introduction.html
[FLEECE]: https://github.com/couchbaselabs/fleece/wiki/Using-Fleece
