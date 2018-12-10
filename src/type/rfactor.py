#!/usr/bin/env python

import sys
import glob
import subprocess

def rfactor():
	args = sys.argv
	if len(args) < 2:
		print("Need 2 args")
		return

	inbase = args[1]
	outbase = args[2]
	files = glob.glob("%s*" % inbase)
	for f in files:
		iff = f
		off = f.replace(inbase, outbase)
		print("%s -> %s" % (iff, off))
		cmd = "hg mv %s %s" % (iff, off)
		print(cmd)
		subprocess.check_output(cmd, shell=True)
	# print(files)


if __name__ == "__main__":
	rfactor()
