
# Duck Hunt is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.

# Duck Hunt is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Design copyright (C) 1985-2015 Nintendo

# Developed in C++ by:
# 	- Bisi Simone 	 [ bisi.simone (at) gmail (dot) com ]
# 	- Carpino Andrea [ carpocci (at) gmail (dot) com ]
# for studying purposes ONLY, between 2014/2015.

TARGET = ../duck_hunt

OBJECTS = dog.o var_init.o resources.o game_a.o timer.o duck.o game_init.o \
		image_man.o main.o

FLAGS = -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -Wall

DEP = depend_file
RM = rm -rf

################################################################################

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(FLAGS)

-include $(DEP)

################################################################################

.PHONY: depend clean clean-all

depend:
	g++ -MM *.cpp > $(DEP)
	
clean:
	$(RM) *.o $(DEP) *~
	
clean-all:
	$(RM) *.o $(DEP) $(TARGET) *~
