import os
import glob
import time
with open("build.data","wr") as f:
    print f


print glob.glob("src/*.c")
print time.time()
print os.stat("test").st_mtime
