INPUTFILE = main.cpp

OUTPUTFILE = main

RUN = g++ -Wall -DUSEGLEW -g -o $(OUTPUTFILE) $(INPUTFILE) shaderClass.cpp -lmingw32 -mwindows -lfreeglut -lglfw3 -lglew32 -lglu32 -lopengl32 -lm

make:
	$(RUN)

run:
	./$(OUTPUTFILE).exe
