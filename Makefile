# Компилятор. Может быть как и mingw32-g++, так и g++
CXX = g++

# Defin'ы для указания новой версии Windows
CXXFLAGS += -D__USE_MINGW_ANSI_STDIO=0
CXXFLAGS += -D_WIN32_IE=0x501
CXXFLAGS += -DWINVER=0x0501
CXXFLAGS += -D_WIN32_WINNT=0x501
CXXFLAGS += -DUNICODE

# Каталоги .h файлов
CXXFLAGS += -I include

# Это если вы не хотите помещать эти библиотеки в mingw/include, поместите их в локальный include
CXXFLAGS += -I include/agg

# Общие флаги
CXXFLAGS += -O3
CXXFLAGS += -std=c++11
CXXFLAGS += -w
CXXFLAGS += -fpermissive
# CXXFLAGS += -g

# Сильно увеличивает размер, но делает проги независимыми на чужом компе
LINKFLAGS += -static-libgcc -static-libstdc++

# Если эту опцию поместить перед mwindows, то получится консоль с окном
# LINKFLAGS += -mconsole 
LINKFLAGS += -mwindows

# Оптимизация размера
LINKFLAGS += -s 
SIZEFLAGS += -Wl,--gc-sections -Wl,--strip-all 
SIZEFLAGS += -fdata-sections -ffunction-sections
SIZEFLAGS += -ffast-math
SIZEFLAGS += -Wunused
SIZEFLAGS += -flto

# Библиотеки для линковки, они у меня почему-то не работают, поэтому линкую напрямую .a файлы
LIBFLAGS += -lgdi32 -lwinmm -lmsimg32 -lcomctl32 -lcomdlg32 -lole32
#LIBFLAGS += -lstdc++
# Эти надо компилировать самому
LIBFLAGS += -lagg

###############################################################################

OBJECTS = $(patsubst src/%, obj/%, $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)))
# OBJECTS += $(patsubst examples/%.rc, obj/%.o,$(wildcard examples/*.rc))
INCLUDES = $(wildcard include/*.h)
INCLUDES += $(wildcard include/ctrl/*.h)
INCLUDES += $(wildcard include/image/*.h)
INCLUDES += $(wildcard include/window/*.h)
EXAMPLES = $(patsubst examples/%.cpp, obj/%.o,$(wildcard examples/*.cpp))
EXAMPLES += $(patsubst examples/%.cpp, obj/%.o,$(wildcard examples/*/*.cpp))
EXES = $(patsubst obj/%.o, bin/%.exe,$(EXAMPLES))
ALIBS = $(patsubst -l%, D:/ProgFiles/MinGW/lib/lib%.a, $(LIBFLAGS))

###############################################################################

all: dirs $(OBJECTS) $(EXAMPLES) $(EXES)

dirs:
	if not exist "bin" mkdir bin
	if not exist "obj" mkdir obj
	cd bin & if not exist "agg" mkdir agg & cd ..
	cd bin & if not exist "other" mkdir other & cd ..
	cd bin & if not exist "complicated" mkdir complicated & cd ..
	cd bin & if not exist "simple" mkdir simple & cd ..
	cd obj & if not exist "agg" mkdir agg & cd ..
	cd obj & if not exist "other" mkdir other & cd ..
	cd obj & if not exist "complicated" mkdir complicated & cd ..
	cd obj & if not exist "simple" mkdir simple & cd ..

bin/%.exe: $(OBJECTS) obj/%.o
	$(CXX) $(LINKFLAGS) $(SIZEFLAGS) $(CXXFLAGS) $(LIBFLAGS) $(OBJECTS) $(patsubst bin/%.exe, obj/%.o, $@) -o $@ $(ALIBS)

obj/%.o: src/%.cpp $(INCLUDES)
	$(CXX) $(SIZEFLAGS) $(CXXFLAGS) -c $< -o $@

obj/%.o: examples/%.cpp $(INCLUDES)
	$(CXX) $(SIZEFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	if exist "bin" rd bin /S /Q
	if exist "obj" rd obj /S /Q