import os

SRC_C = []
SRC_CPP = []
INC = []

GB_SRC_DIR = os.path.expanduser("~") + "/.arduino15/packages/gamebuino/hardware/samd/1.2.1/"

SCAN_DIRS = [
	["modules/Gamebuino-Meta", 1],
#	[GB_SRC_DIR + "libraries/SPI", 1],
#	[GB_SRC_DIR + "cores", 2],
#	[GB_SRC_DIR + "variants", 2],
]

def addDir(d, do_inc):
	files = os.listdir(d)
	if do_inc > 0:
		if "src" in files:
			INC.append(d + "src")
		else:
			INC.append(d[:-1])
	for file in files:
		if file in ["SdFat", "Adafruit_ZeroDMA"]:
			continue
		f = d + file
		if f.endswith(".c"):
			SRC_C.append(f)
		elif f.endswith(".cpp"):
			SRC_CPP.append(f)
		elif os.path.isdir(f) and not "utility" in files:
			addDir(f+"/", do_inc - 1)
	if "utility" in files:
		addDir(d+"utility/", do_inc - 1)

for d, lib in SCAN_DIRS:
	addDir(d + "/", lib)

print("SRC_C_EXTRA += \\")
for p in SRC_C:
	print("\t" + p + " \\")
print("\n")

print("SRC_CPP_EXTRA += \\")
for p in SRC_CPP:
	print("\t" + p + " \\")
print("\n")

print("INC_EXTRA += \\")
for p in INC:
	print("\t-I" + p + " \\")
print("\n")
