CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./obj/gtcsmain.o ./obj/comport.o ./obj/crcchecker.o ./obj/database.o ./obj/tcpsocket.o ./obj/gtcsprotocol.o

gtcs: gtcsmain.o comport.o crcchecker.o database.o tcpsocket.o gtcsprotocol.o
	${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 

gtcsmain.o : ./gtcsmain.cpp
	${CC} ${INCLUDE} -c $< -o ./obj/gtcsmain.o

comport.o : ./src/comport.cpp 
	${CC} ${INCLUDE} -c $< -o ./obj/comport.o

crcchecker.o : ./source/crcchecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./obj/crcchecker.o

database.o : ./source/database.cpp 
	${CC} ${INCLUDE} -c $< -o ./obj/database.o

tcpsocket.o : ./source/tcpsocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./obj/tcpsocket.o

gtcsprotocol.o : ./source/gtcsprotocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./obj/gtcsprotocol.o

clear:
	rm -f ./obj/*.o
