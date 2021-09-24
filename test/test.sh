#! /bin/bash -e
#
# Convenience script to run `test.py` -- 
# just sets PYTHONPATH to point to the parent dire, so the CouchbaseLite package will be loaded.

SCRIPT_DIR=`dirname $0`
cd "$SCRIPT_DIR"

export PYTHONPATH=..
python3 test.py
