**Zodiac** is an Astrological software for personal use.

**Download the latest version**: [**Windows**](http://sourceforge.net/projects/zodiac-app/files/Zodiac-0.6.2-installer.exe/download) | [**Linux64**](http://sourceforge.net/projects/zodiac-app/files/zodiac-0.6.2.tar.gz/download)



Features:
=========

* Contains different views of horoscope: classical chart, planets view and plain text;
* Provides detail information about each element of horoscope;
* Uses 9 house systems and over 40 aspects for horoscope calculation;
* Allows to view tropical and sidereal zodiacs;
* Compact, dock-based user interface;
* Includes Swiss Ephemeris © library;
* English & Russian language support.


Subprojects:
------------

``Zodiac` project consists of following parts:

* ``zodiac`` - front-end application with tab interface and user-defined files management;
* ``astroprocessor`` - library with classes for astrological calculations and settings management;
* ``swe`` - Swiss Ephemeris Library. Provides positions of planets, houses etc;
* ``chart`` - library for making natal chart;
* ``plain`` - library for making simple text view of horoscope;
* ``planets`` - library for making planets viewer;
* ``fileeditor`` - library for editing horoscope data.


Directories:
------------

* ***bin/*** - application build&run directory.
* ***bin/i18n/*** - localization files for all projects;
* ***bin/images/*** - various astrological images used in application
* ***bin/style/*** - CSS and icons for application
* ***bin/text/*** - interpretations of astrological items
* ***bin/user/*** - collection of user files (File->Open & File->Save)
* ***bin/astroprocessor/***,
* ***bin/chart/***,
* ***bin/fileeditor/***,
* ***bin/plain/***,
* ***bin/planets/***,
* ***bin/swe/*** - files used by appropriate library;
* ***chart/***,
* ***fileeditor/***,
* ***plain/***,
* ***planets/***,
* ***astroprocessor/***,
* ***swe/***,
* ***zodiac/*** - subprojects
* ***nsis/*** - files for Nullsoft Scriptable Install System


Build instructions:
===================

Requirement: Qt > 4.8 (works in Qt 4.8.2 and in Qt 5.2 as well).

Method 1: automatic bundle build
---------------------------------

This method will produce a single executable file with integrated project libraries. It is useful, then qmake can't link that libraries to executable for some reason.

Open **zodiac/zodiac_bundle.pro** in Qt Creator, select a build configuration (Debug or Release) then build (Ctrl+B).

Method 2: automatic separate build
-----------------------------------

This method is tested in Windows only. It will produce a main executable (zodiac) with a few dynamic libraries. Open **all.pro** in Qt Creator, and build it as usual.

Method 3: manual build
-----------------------

This is a variation of 'method 2' that supposes manual building of all subprojects.
Open projects in QtCreator and build them manually in the following order:

    1. swe/swe.pro -> bin/swe.dll;
    2. astroprocessor/astroprocessor.pro -> bin/astroprocessor.dll;
    3. chart/chart.pro -> bin/chart.dll,
    fileeditor/fileeditor.pro -> bin/fileeditor.dll,
    plain/plain.pro -> bin/plain.dll,
    planets/planets.pro -> bin/planets.dll;
    4. zodiac/zodiac.pro -> bin/zodiac.exe.
	
	
Run on Windows:
===============
	
To run application on Windows after build, put following Qt libraries into app directory:

	imageformats/qgif.dll
	imageformats/qico.dll
	imageformats/qjpeg.dll
	platforms/qwindows.dll

In Qt 5.2, they are located in ***C:\Qt\5.2.1\mingw48_32\plugins***.


External links:
===============

For more information, visit

- [SourceForge page](https://sourceforge.net/projects/zodiac-app/)
- [Personal blog](http://www.syslog.pro/tag/zodiac)