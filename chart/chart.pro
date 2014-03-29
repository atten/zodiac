QT += widgets
TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/chart_ru.ts \
               ../bin/i18n/chart_en.ts

SOURCES += src/chart.cpp
HEADERS += src/chart.h

# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor

































