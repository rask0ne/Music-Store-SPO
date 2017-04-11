QT += core
QT -= gui

CONFIG += c++11

TARGET = CLIENT
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Customer.cpp

HEADERS += \
    Customer.h

