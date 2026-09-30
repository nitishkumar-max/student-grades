CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET   := student_grades
SOURCES  := main.cpp Person.cpp
HEADERS  := Person.h InputUtils.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: clean
