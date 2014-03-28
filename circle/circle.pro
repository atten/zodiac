QT += widgets
TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/circle_ru.ts \
               ../bin/i18n/circle_en.ts

SOURCES += src/circle.cpp
HEADERS += src/circle.h

# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor

































