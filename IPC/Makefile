OBJS 	= init.o 
SOURCE	= init.c 	
HEADER  = myRecordDef.h 
OUT  	= init
CC		= gcc
FLAGS   = -g -c 
OBJS2 	= detacher.o 
SOURCE2	= detacher.c 	 
OUT2  	= detacher
OBJS3 	= reader.o 
SOURCE3	= reader.c 	 
OUT3  	= reader
OBJS4 	= writer.o 
SOURCE4	= writer.c 	 
OUT4  	= writer

all:init detacher reader writer 

init.o: init.c
	$(CC) $(FLAGS) init.c
	
init : $(SOURCE) $(OBJS) $(HEADER) 
	$(CC) -o $(OUT) $(SOURCE) -lpthread

detacher.o: detacher.c
	$(CC) $(FLAGS) detacher.c 

detacher : $(SOURCE2) $(OBJS2)	
	$(CC) -g $(OBJS2) -o $(OUT2) -lpthread

reader.o: reader.c
	$(CC) $(FLAGS) reader.c

reader : $(SOURCE3) $(OBJS3)	
	$(CC) -g $(OBJS3) -o $(OUT3) -lpthread

writer.o: writer.c
	$(CC) $(FLAGS) writer.c

writer : $(SOURCE4) $(OBJS4)	
	$(CC) -g $(OBJS4) -o $(OUT4) -lpthread

clean :
	rm -f *.o init detacher reader writer
