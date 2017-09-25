# Компилятор. Может быть как и mingw32-g++, так и g++
CXX = g++

# Defin'ы для указания новой версии Windows
CXXFLAGS += -D__USE_MINGW_ANSI_STDIO=0

# Каталоги .h файлов
CXXFLAGS += -I include

# Общие флаги
CXXFLAGS += -O3
CXXFLAGS += -std=c++11
CXXFLAGS += -w
CXXFLAGS += -fpermissive
# CXXFLAGS += -g

###############################################################################

OBJECTS = $(patsubst src/%, obj/%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))
INCLUDES = $(wildcard include/*.h)
INCLUDES += $(wildcard include/*/*.h)
EXAMPLES = $(patsubst examples/%.cpp, obj/%.o,$(wildcard examples/*.cpp))
EXES = $(patsubst obj/%.o, bin/%.exe,$(EXAMPLES))

###############################################################################

all: dirs $(OBJECTS) $(EXAMPLES) $(EXES)

dirs:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj

bin/%.exe: $(OBJECTS) obj/%.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(patsubst bin/%.exe, obj/%.o, $@) -o $@

obj/%.o: src/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: examples/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist "bin" rd bin /S /Q
	if exist "obj" rd obj /S /Q