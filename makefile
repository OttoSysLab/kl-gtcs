CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./object/GtcsMain.o ./object/ComPort.o ./object/CrcChecker.o ./object/GtcsDatabase.o \
		./object/TcpSocket.o ./object/GtcsBulletin.o ./object/Common.o \
		./object/GtcsAmsProtocol.o ./object/GtcsMcbCommunication.o \
		./object/GtcsBulletinManager.o
LIBS = -lpthread -lsqlite3

gtcs: GtcsMain.o \
      CrcChecker.o \
	  GtcsDatabase.o \
	  TcpSocket.o \
	  ComPort.o \
	  GtcsBulletin.o \
	  Common.o \
	  GtcsBulletinManager.o \
	  GtcsAmsProtocol.o  \
	  GtcsMcbCommunication.o \
	# ${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 
	${CC} -o $@ ${INCLUDE} ${OBJS} ${LIBS}

GtcsMain.o : ./GtcsMain.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMain.o

CrcChecker.o : ./source/CrcChecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/CrcChecker.o

TcpSocket.o : ./source/TcpSocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/TcpSocket.o

GtcsDatabase.o : ./source/GtcsDatabase.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsDatabase.o 

Common.o : ./source/Common.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/Common.o

ComPort.o : ./source/ComPort.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/ComPort.o

GtcsBulletin.o : ./source/GtcsBulletin.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletin.o

GtcsBulletinManager.o : ./source/GtcsBulletinManager.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletinManager.o

GtcsAmsProtocol.o : ./source/GtcsAmsProtocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsAmsProtocol.o

GtcsMcbCommunication.o : ./source/GtcsMcbCommunication.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMcbCommunication.o

clear:
	rm -f ./object/*.o
