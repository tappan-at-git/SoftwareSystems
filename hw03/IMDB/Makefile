# Modify this line to contain the path to the directory that contains
# libmongo.a and src
LIB = /home/etappan/SoftwareSystems/mongo-c-driver/src


# The -I flag tells the compiler where to look for include files.

# To compile tutorial.c, I am following the example in the online
# tutorial at http://api.mongodb.org/c/current/tutorial.html

# There, they compile tutorial.c along with all of the source
# files in the MongoDB C driver.  A better option is to compile
# the MongoDB C driver into a library file (.a or .so), and then
# link tutorial.o with the library.  But for now we'll keep it simple.

# cbacon.o: cbacon.c cbacon.h
# 	gcc -c --std=gnu99 -I$(LIB) $(LIB)/*.c cbacon.c
# cbc: cbacon.o
# 	gcc cbacon.o -o cbc
cbc: cbacon.c cbacon.h
	gcc --std=gnu99 -I$(LIB) $(LIB)/*.c cbacon.c -o cbc