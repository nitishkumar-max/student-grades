# Builds three programs from the same sources, one per container:
#   student_grades_vector, student_grades_list, student_grades_deque
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
SOURCES  := main.cpp Application.cpp Benchmark.cpp FileGenerator.cpp Person.cpp \
            StringUtils.cpp StudentFile.cpp StudentProcessor.cpp TablePrinter.cpp
HEADERS  := $(wildcard *.h)

DEFS_vector :=
DEFS_list   := -DSTUDENT_CONTAINER_LIST
DEFS_deque  := -DSTUDENT_CONTAINER_DEQUE

all: student_grades_vector student_grades_list student_grades_deque

define VARIANT
OBJECTS_$(1) := $(SOURCES:%.cpp=build/$(1)/%.o)

student_grades_$(1): $$(OBJECTS_$(1))
	$$(CXX) $$(CXXFLAGS) -o $$@ $$^

build/$(1)/%.o: %.cpp $$(HEADERS)
	@mkdir -p $$(dir $$@)
	$$(CXX) $$(CXXFLAGS) $$(DEFS_$(1)) -c $$< -o $$@
endef

$(foreach v,vector list deque,$(eval $(call VARIANT,$(v))))

clean:
	rm -rf build student_grades_vector student_grades_list student_grades_deque \
	       student_grades_vector.exe student_grades_list.exe student_grades_deque.exe

.PHONY: all clean
