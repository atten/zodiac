TRANSLATIONS = ../bin/i18n/zodiac_ru.ts \
               ../bin/i18n/zodiac_en.ts

SOURCES += src/main.cpp \
       src/mainwindow.cpp \
    src/help.cpp \
    src/slidewidget.cpp

HEADERS  += src/mainwindow.h \
    src/help.h \
    src/slidewidget.h

## win icon, etc
win32: RC_FILE = app.rc

INCLUDEPATH += ../astroprocessor/include/
