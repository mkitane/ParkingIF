COMP = g++
EDL = g++
RM = rm 
EXE = Parking
CLEAN = efface
CPPFLAGS = -std=c++11 -c -Wall
RMFLAGS = -f
EDLFLAGS = -std=c++11
LIBS = -l tp -l ncurses -l tcl
LIBSPATH = -L/public/tp/tp-multitache
INTERFACE = Mere.h Clavier.h Entree.h
REAL = $(INTERFACE:.h=.cpp)
OBJ = $(INTERFACE:.h=.o)

.PHONY : $(CLEAN)

$(EXE) : $(OBJ)
	$(EDL) -o $(EXE) $(OBJ) $(LIBSPATH) $(LIBS)

%.o : %.cpp
	$(COMP) $(CPPFLAGS) $<

$(CLEAN) :
	$(RM) $(RMFLAGS) *.o $(EXE) core
