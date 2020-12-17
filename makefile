CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./object/GtcsMain.o ./object/ComPort.o ./object/CrcChecker.o ./object/Database.o \
	./object/TcpSocket.o ./object/GtcsBulletin.o ./object/GtcsBulletinManager.o ./object/Common.o \
	./object/GtcsAmsProtocol.o ./object/GtcsMcbCommunication.o

GTCS: GtcsMain.o CrcChecker.o Database.o TcpSocket.o ComPort.o GtcsBulletin.o GtcsMcbCommunication.o\
	GtcsAmsProtocol.o GtcsBulletinManager.o Common.o
	${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 

GtcsMain.o : ./GtcsMain.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMain.o

CrcChecker.o : ./source/CrcChecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/CrcChecker.o

TcpSocket.o : ./source/TcpSocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/TcpSocket.o

Database.o : ./source/Database.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/Database.o

Common.o : ./source/Common.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/Common.o

ComPort.o : ./source/ComPort.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/ComPort.o

GtcsBulletin.o : ./source/GtcsBulletin.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletin.o

GtcsBulletinManager.o : ./source/GtcsBulletinManager.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletinManager.o

GtcsMcbCommunication.o : ./source/GtcsMcbCommunication.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMcbCommunication.o

GtcsAmsProtocol.o : ./source/GtcsAmsProtocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsAmsProtocol.o

clear:
	rm -f ./object/*.o
