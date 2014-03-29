``Zodiac`` is an Astrological software for personal use.

[**Download the latest version**](http://sourceforge.net/projects/zodiac-app/files/latest/download)


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
============

``Zodiac` project consists of following parts:

* ``zodiac`` - front-end application with tab interface and user-defined files management;
* ``astroprocessor`` - library with classes for astrological calculations and settings management;
* ``swe`` - Swiss Ephemeris Library. Provides positions of planets, houses etc;
* ``chart`` - library for making natal chart;
* ``plain`` - library for making simple text view of horoscope;
* ``planets`` - library for making planets viewer;
* ``fileeditor`` - library for editing horoscope data.


Directories:
============

* ***/bin/*** - application build&run directory.
* ***/bin/i18n/*** - localization files for all projects;
* ***/bin/images/*** - various astrological images used in application
* ***/bin/style/*** - CSS and icons for application
* ***/bin/text/*** - interpretations of astrological items
* ***/bin/user/*** - collection of user files (File->Open & File->Save)
* ***/bin/astroprocessor/***,
* ***/bin/chart/***,
* ***/bin/fileeditor/***,
* ***/bin/plain/***,
* ***/bin/planets/***,
* ***/bin/swe/*** - files used by appropriate library;
* ***/chart/***,
* ***/fileeditor/***,
* ***/plain/***,
* ***/planets/***,
* ***/libAstroprocessor/***,
* ***/libSwisseph/***,
* ***/zodiac/*** - sources of QT projects
* ***/nsis/*** - files for Nullsoft Scriptable Install System


Build order:
============

Run QT, open and build projects in the following order:

    1. /libSwisseph/swe.pro -> /bin/swe.dll;
    2. /libAstroprocessor/astroprocessor.pro -> /bin/astroprocessor.dll;
    3. /chart/chart.pro -> /bin/chart.dll,
    /fileeditor/fileeditor.pro -> /bin/fileeditor.dll,
    /plain/plain.pro -> /bin/plain.dll,
    /planets/planets.pro -> /bin/planets.dll;
    4. /zodiac/zodiac.pro -> /bin/zodiac.exe.

To run application on Windows, make sure you put following QT libraries into app directory:

	/bin/imageformats/qgif.dll
	/bin/imageformats/qico.dll
	/bin/imageformats/qjpeg.dll
	/bin/platforms/qwindows.dll

In QT 5.2, they located in ***C:\Qt\5.2.1\mingw48_32\plugins***.


External links:
===============

For more information, visit

- [SourceForge page](https://sourceforge.net/projects/zodiac-app/)
- [Personal blog](http://www.syslog.pro/tag/zodiac)