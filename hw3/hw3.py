import os
if not os.path.exists("bin"):
    os.system("mkdir bin")
os.system("bash hw3.sh")
os.system("bin/genData")
print("genData complete")
os.system("bin/updown1")
print("updown1 complete")
os.system("bin/updown2")
print("updown2 complete")
