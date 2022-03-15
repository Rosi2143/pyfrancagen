#!/usr/bin/python3

import sys, os
sys.path.append(os.getcwd() + "/../pyfranca_cpp")
sys.path.append(os.getcwd() + "/pyfranca_cpp")

from pyfranca_cpp import *

LOCALINPUTDIR = '../lbs/fidl'

FIDL_FILES =  [ '%s/%s' % (LOCALINPUTDIR,s) for s in [
    'avb_audio.fidl',
    'avb_test.fidl',
    'avb_trace.fidl' ] ]

def main():
    for f in FIDL_FILES:
        log ("-------------------------------------------------------")
        log (" ----- PROCESSING %s -----" % f)
        log ("-------------------------------------------------------")
        process_file(f)


if __name__ == "__main__":
    main()

