TEMPLATE	= lib
CONFIG		+= qtopia

TARGET		= mfgtunemode 
DESTDIR     = $$(QPEDIR)/lib

INCLUDEPATH += $(QPEDIR)/include
INCLUDEPATH += $(QPEDIR)/include/qpe
INCLUDEPATH += $(KERNELINCLUDE_PATH)/
INCLUDEPATH += $(UNILIBS_PATH)/

HEADERS += mfgtunemode.h \
           interpolation.h \
           dspcontrol.h \
           antennatunetable.h \
           mfgtunedefine.h \
           receiveconditionswitch.h \
           toolformfgtune.h \
           calibratedata.h \
           dspregisterdebug.h \


           
SOURCES += mfgtunemode.cpp \
           interpolation.cpp \
           dspcontrol.cpp \
           antennatunetable.cpp \
           receiveconditionswitch.cpp \
           toolformfgtune.cpp \
           calibratedata.cpp \
           dspregisterdebug.cpp \

