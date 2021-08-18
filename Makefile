CXX = em++
EXEC = bin\homicide.js
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(addprefix obj/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
LIB = lib/
INCLUDE = inc/
FLAGS = -sUSE_SDL=2
STD_VERSION = c++17


all : $(EXEC)
resources : obj/resources.o

obj/%.o : src/%.cpp
	@echo compile $@ from $<
	@$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE)

obj/%.o : src/*/%.cpp
	@echo compile $@ from $<
	@$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE)

$(EXEC) : $(OBJ)
	@echo building the executable ...
	@$(CXX) -std=$(STD_VERSION) obj/*.o -I $(INCLUDE) -L $(LIB) -o $(EXEC) $(FLAGS)
	@echo executable bluid, ready to launch

obj/resources.o : src/resources/resources.rc
	@echo build resources file
	@windres $< -o $@ 

clean:
	del obj\*.o

launch:
	@$(EXEC)

info:
	@echo -----------------------------------------------------
	@echo compiling info :                
	@echo 	compiler                     : $(CXX)
	@echo 	compiler standart version    : $(STD_VERSION)
	@echo 	include path                 : $(INCLUDE)
	@echo 	flags                        : $(FLAGS)
	@echo 	libs directory               : $(LIB)
	@echo 	executable output            : $(EXEC)
	@echo -----------------------------------------------------
