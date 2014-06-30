**Zodiac** is an Astrological software for personal use.

**Download the latest version**: [**Windows**](http://sourceforge.net/projects/zodiac-app/files/Zodiac-0.7.0-installer.exe/download) | [**Linux64**](http://sourceforge.net/projects/zodiac-app/files/zodiac-0.7.0.tar.gz/download)



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
* ``details`` - library for displaying planet properties;
* ``fileeditor`` - library for editing horoscope data.


Content of subdirs:
------------

* ***bin/*** - executable, libraries and other application files.
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
* ***bin/swe/*** - files used by Swiss Ephemeris library;
* ***chart/***, ***fileeditor/***, ***plain/***, ***planets/***, ***details/***, ***astroprocessor/***, ***swe/***, ***zodiac/*** - subprojects
* ***nsis/*** - files for Nullsoft Scriptable Install System


Build instructions:
===================

Requirement: Qt > 4.8 (works in Qt 4.8.2 and in Qt 5.3 as well).

Method 1: automatic bundle build
---------------------------------

This method will produce a single executable file with integrated project libraries.

Open **zodiac/zodiac_bundle.pro** in Qt Creator, select a build configuration (Debug or Release) then build (Ctrl+B).

Method 2: automatic separate build (Windows only)
-----------------------------------

Open **all.pro** in Qt Creator, and build it as usual. It will produce a main executable (zodiac) with a few dynamic libraries.

Method 3: manual build
-----------------------

This is a variation of 'method 2' that supposes manual building of all subprojects.
Open projects in QtCreator and build them manually in the following order:

    1. swe/swe.pro
    2. astroprocessor/astroprocessor.pro
    3. chart/chart.pro
    fileeditor/fileeditor.pro
    plain/plain.pro
    planets/planets.pro
	planets/details.pro
    4. zodiac/zodiac.pro
	
	
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