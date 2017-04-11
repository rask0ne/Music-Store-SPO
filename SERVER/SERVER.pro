QT += core
QT -= gui

CONFIG += c++11

TARGET = SERVER
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    shop.cpp

HEADERS += \
    shop.h

