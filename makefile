CC = g++
INCLUDE = -I"./include" -I"../include"
OBJS = ./object/GtcsMain.o ./object/ComPort.o ./object/CrcChecker.o ./object/GtcsDatabase.o \
		./object/GtcsTcpSocket.o ./object/GtcsGloabDefine.o ./object/GtcsBulletin.o ./object/Common.o \
		./object/GtcsAmsProtocol.o ./object/GtcsMcbComm.o \
		./object/Manager.o 

LIBS = -lpthread -lsqlite3

gtcsdriver: GtcsMain.o \
      CrcChecker.o \
	  GtcsDatabase.o \
	  GtcsTcpSocket.o \
	  ComPort.o \
	  GtcsGloabDefine.o \
	  GtcsBulletin.o \
	  Common.o \
	  GtcsAmsProtocol.o  \
	  GtcsMcbComm.o \
	  Manager.o \
	# ${CC} -pthread -o $@ ${INCLUDE} ${OBJS} 
	${CC} -o $@ ${INCLUDE} ${OBJS} ${LIBS}

GtcsMain.o : ./GtcsMain.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMain.o

CrcChecker.o : ./source/CrcChecker.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/CrcChecker.o

GtcsTcpSocket.o : ./source/GtcsTcpSocket.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsTcpSocket.o

GtcsDatabase.o : ./source/GtcsDatabase.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsDatabase.o 

Common.o : ./source/Common.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/Common.o

ComPort.o : ./source/ComPort.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/ComPort.o

GtcsGloabDefine.o : ./source/GtcsGloabDefine.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsGloabDefine.o

GtcsBulletin.o : ./source/GtcsBulletin.cpp
	${CC} ${INCLUDE} -c $< -o ./object/GtcsBulletin.o

GtcsAmsProtocol.o : ./source/GtcsAmsProtocol.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsAmsProtocol.o

GtcsMcbComm.o : ./source/GtcsMcbComm.cpp 
	${CC} ${INCLUDE} -c $< -o ./object/GtcsMcbComm.o

Manager.o : ./source/Manager.cpp
	${CC} ${INCLUDE} -c $< -o ./object/Manager.o

clear:
	rm -f ./object/*.o
