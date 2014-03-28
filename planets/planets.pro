QT += widgets declarative

TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/planets_ru.ts \
               ../bin/i18n/planets_en.ts

SOURCES += src/planets.cpp \
    src/expandwidget.cpp
HEADERS += src/planets.h \
       src/expandwidget.h


# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor



































