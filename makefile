CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./object/gtcsmain.o ./object/comport.o ./object/crcchecker.o ./object/gtcsdatabase.o \
		./object/gtcstcpsocket.o ./object/gtcsgloabdefine.o ./object/gtcsbulletin.o ./object/common.o \
		./object/gtcsamsprotocol.o ./object/gtcsmcbcomm.o \
		./object/manager.o 

LIBS = -lpthread -lsqlite3

gtcsdriver: gtcsmain.o \
      crcchecker.o \
	  gtcstcpsocket.o \
	  gtcsdatabase.o \
	  comport.o \
	  gtcsgloabdefine.o \
	  gtcsbulletin.o \
	  common.o \
	  gtcsamsprotocol.o  \
	  gtcsmcbcomm.o \
	  manager.o \
	# ${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 
	${CC} -o $@ ${INCLUDE} ${OBJS} ${LIBS}

gtcsmain.o : ./gtcsmain.cpp
	${CC} ${INCLUDE} -c $< -o ./object/gtcsmain.o

crcchecker.o : ./source/crcchecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/crcchecker.o

gtcstcpsocket.o : ./source/gtcstcpsocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/gtcstcpsocket.o

gtcsdatabase.o : ./source/gtcsdatabase.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/gtcsdatabase.o 

common.o : ./source/common.cpp
	${CC} ${INCLUDE} -c $< -o ./object/common.o

comport.o : ./source/comport.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/comport.o

gtcsgloabdefine.o : ./source/gtcsgloabdefine.cpp
	${CC} ${INCLUDE} -c $< -o ./object/gtcsgloabdefine.o

gtcsbulletin.o : ./source/gtcsbulletin.cpp
	${CC} ${INCLUDE} -c $< -o ./object/gtcsbulletin.o

gtcsamsprotocol.o : ./source/gtcsamsprotocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/gtcsamsprotocol.o

gtcsmcbcomm.o : ./source/gtcsmcbcomm.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/gtcsmcbcomm.o

manager.o : ./source/manager.cpp
	${CC} ${INCLUDE} -c $< -o ./object/manager.o

clear:
	rm -f ./object/*.o
