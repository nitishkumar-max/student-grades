CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET   := student_grades
SOURCES  := main.cpp Application.cpp Benchmark.cpp FileGenerator.cpp Person.cpp \
            StringUtils.cpp StudentFile.cpp StudentProcessor.cpp TablePrinter.cpp
OBJECTS  := $(SOURCES:.cpp=.o)
HEADERS  := $(wildcard *.h)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJECTS)

.PHONY: clean
