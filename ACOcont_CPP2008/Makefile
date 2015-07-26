TARGET = aco-r
CXXFLAGS = -ansi -O3 -g
OBJS = Timer.o Solution.o Functions.o 
MCOBJS = Timer.o Solution.o Functions.o Colony.o

all: ${TARGET}

aco-r: aco-r.cpp $(OBJS) 
	${CXX} ${CXXFLAGS} $^ -o $@ 

aco-r-new: aco-r-new.cpp $(MCOBJS)
	${CXX} ${CXXFLAGS} $^ -o $@ 

clean:
	@rm -f *~ *.o ${TARGET} core

