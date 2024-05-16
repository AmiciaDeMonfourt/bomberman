TARGET = bomber
COMPILER = C:\MinGW\bin\g++
INCLUDE_PATHS = -I SDL/include -L SDL/lib/ -I source -I source/scenes
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2  -lSDL2_image -lSDL2_ttf -lSDL2_mixer
COMPILER_FLAGS = -w -g
OUTPUT = -o a

PREF_SOURCE = ./source/
PREF_SCENE = ./source/scenes/
PREF_UTILS = ./source/utils/
PREF_ENTITY = ./source/entity/

#функция получения списка из cpp файлов в выбранной дериктории(1 переданный аргумент)
define GET_CPP
    $(wildcard $(1)*.cpp)
endef

#список исходных и объектных файлов
CPP := $(call GET_CPP, $(PREF_SOURCE)) $(call GET_CPP, $(PREF_SCENE)) $(call GET_CPP, $(PREF_UTILS)) $(call GET_CPP, $(PREF_ENTITY)) 
OBJ := $(CPP:%.cpp=%.o)

#компилируем все исходные файлы в объектные
%.o : %.cpp
	$(COMPILER) -c $< -o $@
 
$(TARGET) : $(OBJ)
	$(COMPILER) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(CPP) $(LINKER_FLAGS) $(OUTPUT)


debug : $(OBJ)
	$(COMPILER) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(CPP) $(LINKER_FLAGS) $(OUTPUT)
	gdb a
	run

clean:
	rm -f $(TARGET) $(OBJ)

cleanObj:
	rm -f $(OBJ)
