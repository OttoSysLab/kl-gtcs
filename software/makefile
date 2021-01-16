TARGET = gtcsdriver
CC = g++

SRCS :=
INCLUDES :=
LIBS :=
OBJS :=
DEPS := 
CFLAGS :=
CMD := 

INCLUDE += -I"./include" 
INCLUDE += -I"../include"

OBJS += ./object/gtcsmain.o 
OBJS += ./object/comport.o 
OBJS += ./object/crcchecker.o 
OBJS += ./object/gtcsdatabase.o 
OBJS += ./object/gtcstcpsocket.o 
OBJS += ./object/gtcsgloabdefine.o 
OBJS += ./object/gtcsbulletin.o 
OBJS += ./object/common.o 
OBJS += ./object/gtcsamsprotocol.o 
OBJS += ./object/gtcsmcbcomm.o
OBJS += ./object/manager.o 

CMD += gtcsmain.o 
CMD += comport.o 
CMD += crcchecker.o 
CMD += gtcsdatabase.o 
CMD += gtcstcpsocket.o 
CMD += gtcsgloabdefine.o 
CMD += gtcsbulletin.o 
CMD += common.o 
CMD += gtcsamsprotocol.o 
CMD += gtcsmcbcomm.o
CMD += manager.o

LIBS += -lpthread 
LIBS += -lsqlite3

all:$(OBJS)
	@${CC} ${INCLUDE} ${OBJS} ${LIBS} -o ${TARGET}

build: $(CMD)

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
