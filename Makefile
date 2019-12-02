UNAME := $(shell uname)
BASE_DIR := $(shell pwd)
NPROCS := 1

ifeq ($(UNAME),Linux)
	NPROCS := $(shell grep -c ^processor /proc/cpuinfo)
else ifeq ($(UNAME),Darwin) # Assume Mac OS X
	NPROCS := $(shell system_profiler | awk '/Number Of CPUs/{print $4}{next;}')
endif

.PHONY: auto compile build clean run test

auto: build

compile:
	cd build; make -j$(NPROCS)

build:
	mkdir -p build
	cd build; CC=clang CXX=clang++ cmake -G "Unix Makefiles" ..; make -j$(NPROCS)

clean:
	rm -rf build

run:
	cd build/$(day); ./$(day)

test:
	cd build/$(day); ./$(day)-test

cppcheck:
	cppcheck --project=$(BASE_DIR)/build/compile_commands.json -j$(NPROCS) --enable=all --output-file=build/cppcheck-out.txt
	cat build/cppcheck-out.txt

valgrind:
	valgrind --leak-check=full --log-file=build/valgrind-out.txt ./build/$(day)/$(day)
	cat build/valgrind-out.txt