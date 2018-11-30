Name "UraniumX (${URX_ARCH})"

RequestExecutionLevel highest
SetCompressor /SOLID lzma

# General Symbol Definitions
!define REGKEY "SOFTWARE\UraniumX"
!define VERSION 1.0.1
!define COMPANY "UraniumX"
!define URL https://uranium-x.com/

# MUI Symbol Definitions
!define MUI_ICON "../../src/qt/res/icons/bitcoin.ico"
# !define MUI_WELCOMEFINISHPAGE_BITMAP "../../share/pixmaps/nsis-wizard.bmp"
# !define MUI_HEADERIMAGE
# !define MUI_HEADERIMAGE_RIGHT
# !define MUI_HEADERIMAGE_BITMAP "../../share/pixmaps/nsis-header.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY ${REGKEY}
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "UraniumX"
!define MUI_FINISHPAGE_RUN $INSTDIR\bin\uraniumx-qt.exe
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
# !define MUI_UNWELCOMEFINISHPAGE_BITMAP "../../share/pixmaps/nsis-wizard.bmp"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

# Included files
!include Sections.nsh
!include MUI2.nsh
!include x64.nsh

# Variables
Var StartMenuGroup

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE English

# Installer attributes
OutFile "../../dist/uraniumx-${VERSION}-${URX_ARCH}-setup.exe"
InstallDir "${URX_INSTDIR}"

CRCCheck on
XPStyle on
BrandingText " "
ShowInstDetails show
VIProductVersion ${VERSION}.0
VIAddVersionKey ProductName "UraniumX"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyName "${COMPANY}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey FileDescription ""
VIAddVersionKey LegalCopyright ""
InstallDirRegKey HKCU "${REGKEY}" Path
ShowUninstDetails show

# Installer sections
Section -Main SEC0000
    SetOutPath $INSTDIR
    SetOverwrite on
    File /r "${URX_DIST_DIR}\*.*"
    File /oname=COPYING.txt "..\..\COPYING"
    File /oname=README.txt "..\..\doc\README_windows.txt"
    SetOutPath $INSTDIR\doc
    File /r "..\..\doc\*.*"
    SetOutPath $INSTDIR
    WriteRegStr HKCU "${REGKEY}\Components" Main 1
SectionEnd

Section -post SEC0001
    WriteRegStr HKCU "${REGKEY}" Path $INSTDIR
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\UraniumX.lnk" $INSTDIR\bin\uraniumx-qt.exe
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\UraniumX (testnet).lnk" "$INSTDIR\bin\uraniumx-qt.exe" "-testnet" "$INSTDIR\bin\uraniumx-qt.exe" 1
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\Uninstall.lnk" $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" DisplayName "UraniumX"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" DisplayVersion "${VERSION}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" Publisher "${COMPANY}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" URLInfoAbout "${URL}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" DisplayIcon $INSTDIR\uninstall.exe
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" NoModify 1
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX" NoRepair 1
    WriteRegStr HKCR "uraniumx" "URL Protocol" ""
    WriteRegStr HKCR "uraniumx" "" "URL:UraniumX"
    WriteRegStr HKCR "uraniumx\DefaultIcon" "" "$INSTDIR\bin\uraniumx-qt.exe"
    WriteRegStr HKCR "uraniumx\shell\open\command" "" '"$INSTDIR\bin\uraniumx-qt.exe" "%1"'
SectionEnd

;--------------------------------
; Desktp Shortcut Section
Section -post SEC0002
    SetShellVarContext current
    CreateShortCut "$DESKTOP\UraniumX.lnk" "$INSTDIR\bin\uraniumx-qt.exe"
SectionEnd


# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKCU "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
Section /o -un.Main UNSEC0000
    RMDir /r /REBOOTOK "$INSTDIR\bin"
    RMDir /r /REBOOTOK "$INSTDIR\doc"
    RMDir /r /REBOOTOK "$INSTDIR\include"
    RMDir /r /REBOOTOK "$INSTDIR\lib"
    RMDir /r /REBOOTOK "$INSTDIR\share"
    Delete /REBOOTOK "$INSTDIR\COPYING.txt"
    Delete /REBOOTOK "$INSTDIR\README.txt"
   
    DeleteRegValue HKCU "${REGKEY}\Components" Main
SectionEnd

Section -un.post UNSEC0001
    DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\UraniumX"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Uninstall.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\UraniumX.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\UraniumX (testnet).lnk"
    Delete /REBOOTOK "$SMSTARTUP\UraniumX.lnk"
    Delete /REBOOTOK "$DESKTOP\UraniumX.lnk"
    Delete /REBOOTOK $INSTDIR\uninstall.exe
    Delete /REBOOTOK $INSTDIR\debug.log
    Delete /REBOOTOK $INSTDIR\db.log
    DeleteRegValue HKCU "${REGKEY}" StartMenuGroup
    DeleteRegValue HKCU "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKCU "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKCU "${REGKEY}"
    DeleteRegKey HKCR "uraniumx"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
    Push $R0
    StrCpy $R0 $StartMenuGroup 1
    StrCmp $R0 ">" no_smgroup
no_smgroup:
    Pop $R0
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
FunctionEnd

# Uninstaller functions
Function un.onInit
    ReadRegStr $INSTDIR HKCU "${REGKEY}" Path
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
    !insertmacro SELECT_UNSECTION Main ${UNSEC0000}
FunctionEnd
