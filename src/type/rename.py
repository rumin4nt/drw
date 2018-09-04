import glob
import subprocess

files = glob.glob("*.c")
files += glob.glob("*.cpp")
files += glob.glob("*.h")

for f in files:
	cmd = "hg mv %s %s" % (f, f.replace("drw_font","drw_text"))
	print(cmd)
	subprocess.check_output(cmd, shell=True)

#print(files)
