#This is a comment

#Compiler to use
CC=gcc
#Flags, for example to specify include directories
CFLAGS=-I.
#Dependencies, like header files
DEPS = include/%.h
#Object files to build and link to obtain target executable
OBJ = event-driven.o
#Name of target executable
TARGET = event-driven

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
	time ./$(TARGET) < test_100000.txt > output.txt
	diff output.txt golden_output_100000.txt

archive: clean
	@read -p 'Roll No: ' r ; \
	tar -czf ../dsa-submit-$$r.tgz -C .. `basename ${PWD}` ; \
	echo "Created the archive file ../dsa-submit-$$r.tgz. Please submit to Moodle."
