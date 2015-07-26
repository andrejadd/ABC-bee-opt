# CCFLAGS : -DDBUG -DTRACE -DDBUGV 

UNAME = $(shell uname -o)

CC = gcc 
CCFLAGS += -ggdb -DMAIN

PYTHON = python2.2
CCFLAGS += -I$(HOME)/usr/include $(FLAGS)

OBJS = pso_globals.o pso.o optfunctions.o bird.o swarm.o tools.o pso_statistics.o movpeaks.o
HDRS = $(OBJS:.o=.h)


%.o : %.c %.h
	$(CC) -c $(CCFLAGS) $< -o $@

%.o : %.cpp %.h
	$(CC) -c $(CCFLAGS) $< -o $@


runpso.o: runpso.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

java/psowrapper/psojava_wrap.o:
	${MAKE} java

swig : pso.i pso.cpp
	swig -python -c++ -o pso_wrap.cpp pso.i 
	$(CC) $(CCFLAGS) -c pso_wrap.cpp -I/usr/include/$(PYTHON)/

java : psojava.i pso.cpp 
	swig -java -c++ -package psowrapper -o java/psowrapper/psojava_wrap.cpp psojava.i 
	$(CC) $(CCFLAGS) -c java/psowrapper/psojava_wrap.cpp -I/C/Programme/Java/j2sdk1.4.2/include/ -I/C/Programme/Java/j2sdk1.4.2/include/win32
	mv psojava_wrap.o java/psowrapper

javamodule: $(OBJS) java/psowrapper/psojava_wrap.o
	$(CC) -shared $(OBJS) java/psowrapper/psojava_wrap.o -lgsl -lgslcblas -lstdc++ -o java/libpso.so

shared : $(OBJS) pso_wrap.o
	$(CC) -shared $(OBJS) pso_wrap.o -L$(HOME)/usr/lib -lgsl -lgslcblas -lstdc++ -L/usr/lib/$(PYTHON)/config -l$(PYTHON)  -o _pso.so

pso : $(OBJS)
	$(CC) $(OBJS) -o pso  -L$(HOME)/usr/lib -lgsl -lgslcblas -lstdc++ $(CCFLAGS)

all : $(OBJS)
	${MAKE} swig shared
	cp _pso.so pso.py ../evaluate/

clean :
	-rm $(OBJS)

stdout :
	CCFLAGS += -DSTDOUT
	clean all


#make clean; make FLAGS=-DMAIN runpso
runpso : $(OBJS) runpso.o 
	$(CC) $(OBJS) -L$(HOME)/usr/lib runpso.o -o runpso  -lgsl -lgslcblas -lstdc++

lsearch : $(OBJS) lsearch.o
	$(CC) $(OBJS) -L$(HOME)/usr/lib lsearch.o -o lsearch -lgsl -lgslcblas -lstdc++

