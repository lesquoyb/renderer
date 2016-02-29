TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tgaimage.cpp

HEADERS += \
    texture.h \
    tgaimage.h \
    tools.h \
    triangle.h \
    vertex.h \
    vertexSwarm.h \
    matrix4.h
