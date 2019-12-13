# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = NetworkTool
DESTDIR = ./x64/Debug
CONFIG += debug
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += .
HEADERS += ./CommonSocket.h \
    ./MainWindow.h \
    ./FramelessWidget.h \
    ./tabTCPServer.h \
    ./tabTCPClient.h \
    ./tabUDPServer.h
SOURCES += ./CommonSocket.cpp \
    ./FramelessWidget.cpp \
    ./main.cpp \
    ./MainWindow.cpp \
    ./tabTCPClient.cpp \
    ./tabTCPServer.cpp \
    ./tabUDPServer.cpp
FORMS += ./FramelessWidget.ui \
    ./MainWindow.ui \
    ./tabTCPClient.ui \
    ./tabTCPServer.ui \
    ./tabUDPServer.ui
RESOURCES += NetworkTool.qrc
