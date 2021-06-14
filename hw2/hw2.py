import os
if not os.path.exists("bin"):
    os.system("mkdir bin")
os.system("bash hw2.sh")
os.system("bin/genData")
os.system("bin/shm")
os.system("bin/verify")
