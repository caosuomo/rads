# Compile and link the entire library and the bundled software.
# The target system can be specified in the command line,
# e.g., make target=win.
# Use "make library" to compile the library only, not the bundled software.
# This makefile was prepared by Pawel Pilarczyk.

TARGET = ${target}

.PHONY: CHomP
CHomP:
ifeq (${TARGET},)
	${MAKE} -C make
else
	${MAKE} -C make target=${TARGET}
endif

.PHONY: library
library:
ifeq (${TARGET},)
	${MAKE} -C make library
else
	${MAKE} -C make target=${TARGET} library
endif

.PHONY: clean
clean:
ifeq (${TARGET},)
	${MAKE} -C make clean
else
	${MAKE} -C make target=${TARGET} clean
endif

