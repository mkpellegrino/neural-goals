CPPFLAGS=--std=c++11 -arch x86_64 -m64
DEBUG=-g -DDEBUG
OPT=-O3

BIN_DIR=./
LIB_DIR=./
LST_DIR=./

neural-goals : neural-goals.cpp
	g++ neural-goals.cpp $(DEBUG) $(CPPFLAGS) -o $(BIN_DIR)neural-goals-debug
	g++ neural-goals.cpp $(OPT) $(CPPFLAGS) -o $(BIN_DIR)neural-goals
	$(BIN_DIR)neural-goals > neural-goals.html

clean :
	rm -fR *~
	rm -fR *.*~
	rm -fR $(LIB_DIR)*.o 
	rm -f $(BIN_DIR)neural-goals
	rm -f $(BIN_DIR)neural-goals.html
	rm -f $(BIN_DIR)neural-goals-debug
	rm -fR $(BIN_DIR)neural-goals-debug.dSYM
	rm -fR $(LST_DIR)*.lst 
