QT += widgets
TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/plain_ru.ts

SOURCES += \
    src/plain.cpp
HEADERS += \
    src/plain.h


# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor































