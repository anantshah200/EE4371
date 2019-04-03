#This is a comment

#Compiler to use
CC=gcc
#Flags, for example to specify include directories
CFLAGS=-I.
#Dependencies, like header files
DEPS = include/%.h
#Object files to build and link to obtain target executable
OBJ = islands.o
#Name of target executable
TARGET = islands

#Creates object files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# The first "target" listed below is the default that gets built
# if nothing else is specified.

#Links object files to create executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Declare "PHONY" targets: these don't build anything
.PHONY: clean test

#Cleans all compilation output files
clean:
	rm -f *.o *~ $(TARGET) output.txt

#Test-runs the executable
test: $(TARGET)
	./$(TARGET) < test1.in > output.txt
	diff output.txt test1.out
	./$(TARGET) < test2.in > output.txt
	diff output.txt test2.out
	./$(TARGET) < test3.in > output.txt
	diff output.txt test3.out
	./$(TARGET) < test4.in > output.txt
	diff output.txt test4.out
	./$(TARGET) < test5.in > output.txt
	diff output.txt test5.out
	./$(TARGET) < test6.in > output.txt
	diff output.txt test6.out
	./$(TARGET) < test7.in > output.txt
	diff output.txt test7.out
	./$(TARGET) < test8.in > output.txt
	diff output.txt test8.out
	./$(TARGET) < test9.in > output.txt
	diff output.txt test9.out
	./$(TARGET) < test10.in > output.txt
	diff output.txt test10.out

archive: clean
	@tar -czf ../dsa-submit.tgz -C .. `basename ${PWD}`
	@echo "Created the archive file ../dsa-submit.tgz.  Please submit to Moodle."
