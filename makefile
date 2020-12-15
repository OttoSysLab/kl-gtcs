CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./object/GtcsMain.o ./object/ComPort.o ./object/CrcChecker.o ./object/Database.o \
	./object/TcpSocket.o ./object/GtcsBulletin.o ./object/GtcsMcbProtocol.o ./object/GtcsAmsProtocol.o

GTCS: GtcsMain.o CrcChecker.o Database.o TcpSocket.o ComPort.o GtcsBulletin.o GtcsMcbProtocol.o\
	GtcsAmsProtocol.o
	${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 

GtcsMain.o : ./GtcsMain.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMain.o

CrcChecker.o : ./source/CrcChecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/CrcChecker.o

TcpSocket.o : ./source/TcpSocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/TcpSocket.o

Database.o : ./source/Database.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/Database.o

ComPort.o : ./source/ComPort.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/ComPort.o

GtcsBulletin.o : ./source/GtcsBulletin.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletin.o

GtcsMcbProtocol.o : ./source/GtcsMcbProtocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMcbProtocol.o

GtcsAmsProtocol.o : ./source/GtcsAmsProtocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsAmsProtocol.o

clear:
	rm -f ./object/*.o
