!define PRODUCT 'Zodiac'
!define VERSION '0.7.1'

!include FontReg.nsh
!include FontName.nsh
!include WinMessages.nsh

Name ${Product}

OutFile "${Product}-${VERSION}-installer.exe"
Icon "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
InstallDir "$PROGRAMFILES\Zodiac"
InstallDirRegKey HKLM "Software\Zodiac" "Install_Dir"
RequestExecutionLevel admin


;--------------------------------
!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_WELCOMEFINISHPAGE_BITMAP "left.bmp"
!define MUI_HEADERIMAGE_BITMAP "top.bmp"
;!insertmacro MUI_LANGUAGE "Russian"

!define MUI_WELCOMEPAGE_TITLE "Установка программы ${PRODUCT} ${VERSION}"
!define MUI_WELCOMEPAGE_TEXT "Установщик проведет вас через все этапы установки программы ${PRODUCT} ${VERSION} на ваш компьютер."

# These indented statements modify settings for MUI_PAGE_FINISH
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN "$INSTDIR\zodiac.exe"
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT "Запустить приложение"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license-ru.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "Russian"

;--------------------------------

; The stuff to install
Section "Файлы программы" SecMain

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File ..\bin\Qt*.dll
  File ..\bin\icu*.dll
  File ..\bin\lib*.dll
  File ..\bin\zodiac.exe
  
  SetOutPath "$INSTDIR\astroprocessor"
  File ..\bin\astroprocessor\*.csv
  
  SetOutPath "$INSTDIR\fileeditor"
  File ..\bin\fileeditor\*
  
  SetOutPath "$INSTDIR\imageformats"
  File ..\bin\imageformats\*.dll

  SetOutPath "$INSTDIR\i18n"
  File ..\bin\i18n\*.qm
  
  SetOutPath "$INSTDIR\images\aspects"
  File ..\bin\images\aspects\*
  
  SetOutPath "$INSTDIR\images\planets"
  File ..\bin\images\planets\*
  
  SetOutPath "$INSTDIR\images\planets-mini"
  File ..\bin\images\planets-mini\*
  
  SetOutPath "$INSTDIR\images\signs"
  File ..\bin\images\signs\*
  
  SetOutPath "$INSTDIR\chart"
  File ..\bin\chart\*
  
  SetOutPath "$INSTDIR\plain"
  File ..\bin\plain\*
  
  SetOutPath "$INSTDIR\planets"
  File ..\bin\planets\*
  
  SetOutPath "$INSTDIR\details"
  File ..\bin\details\*
  
  SetOutPath "$INSTDIR\platforms"
  File ..\bin\platforms\*
  
  SetOutPath "$INSTDIR\style"
  File ..\bin\style\*
  
  SetOutPath "$INSTDIR\swe"
  File ..\bin\swe\*
  
  SetOutPath "$INSTDIR\text\en"
  File ..\bin\text\en\*
  
  SetOutPath "$INSTDIR\text\ru"
  File ..\bin\text\ru\*
  
  SetOutPath "$INSTDIR\user"
  File "..\bin\user\Brad Pitt.dat"
  File "..\bin\user\Diana, Princess of Wales.dat"
  File "..\bin\user\Leonardo Dicaprio.dat"
  File "..\bin\user\Marilyn Monroe.dat"
  File "..\bin\user\Vladimir Putin.dat"
  File "..\bin\user\Yuri Gagarin.dat"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM Software\Zodiac "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zodiac" "DisplayName" "Zodiac"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zodiac" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zodiac" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zodiac" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

Section "Шрифты" SecFonts
	SetOutPath "$FONTS"
	StrCpy $FONT_DIR $FONTS
	!insertmacro InstallTTFFont 'fonts\Almagest.ttf'
	!insertmacro InstallTTFFont 'fonts\DejaVuSans.ttf'
	!insertmacro InstallTTFFont 'fonts\DejaVuSansCondensed.ttf'
	!insertmacro InstallTTFFont 'fonts\DejaVuSerif.ttf'
	;File fonts\*.ttf
SectionEnd

; Optional section (can be disabled by the user)
Section "Ярлыки в меню Пуск" SecFolder
  CreateDirectory "$SMPROGRAMS\Zodiac"
  CreateShortCut "$SMPROGRAMS\Zodiac\Удалить.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Zodiac\Запуск.lnk" "$INSTDIR\zodiac.exe" "" "$INSTDIR\zodiac.exe" 0
SectionEnd

Section "Ярлык на рабочий стол" SecIco
  SetOutPath "$DESKTOP"
  CreateShortCut "$DESKTOP\Zodiac.lnk" "$INSTDIR\zodiac.exe"
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Remove directories used
  RMDir /r "$SMPROGRAMS\Zodiac"
  RMDir /r "$INSTDIR"
  
  Delete "$DESKTOP\Zodiac.lnk"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zodiac"
  DeleteRegKey HKLM SOFTWARE\Zodiac
SectionEnd

;--------------------------------
;Descriptions

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Файлы программы, обязательные для установки."
	!insertmacro MUI_DESCRIPTION_TEXT ${SecFonts} "Шрифты, необходимые для корректного отображения текста и значков."
	!insertmacro MUI_DESCRIPTION_TEXT ${SecFolder} "Папка с ярлыками для запуска или удаления программы."
	!insertmacro MUI_DESCRIPTION_TEXT ${SecIco} "Ярлык на рабочий стол."
  !insertmacro MUI_FUNCTION_DESCRIPTION_END