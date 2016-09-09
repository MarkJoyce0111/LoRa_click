TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    ../src/lora.c \
    ../src/lora_hal.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../include/lora.h \
    ../include/lora_hal.h

INCLUDEPATH += \
    ../include
