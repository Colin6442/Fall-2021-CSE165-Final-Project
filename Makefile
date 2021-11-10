INPUTFILE = test.cpp

OUTPUTFILE = main

RUN = g++ -Wall -DUSEGLEW -o $(OUTPUTFILE) $(INPUTFILE) -lmingw32 -lSDLmain -lSDL -mwindows -lSDL_mixer -lfreeglut -lglfw3 -lglew32 -lglu32 -lopengl32 -lm

make:
	$(RUN)

run:
	./$(OUTPUTFILE).exe
