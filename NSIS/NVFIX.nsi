; *******************************************
; *  NVFIX v1.0.0 installer                 *
; *******************************************

; Includes
; *******************************************
    !include "MUI.nsh"
    !include "Library.nsh"
    !include "Includes\Mutex.nsh"
    !include "Includes\Admin.nsh"
; *******************************************

; General
; *******************************************
    Name "NVFIX"
    OutFile "NVFIX.exe"

    XPStyle on

    InstallDir "$PROGRAMFILES\NVFIX"
    InstallDirRegKey HKLM "Software\NVFIX" "Install Path"
; *******************************************

; Interface Configuration
; *******************************************
    !define MUI_ABORTWARNING
    !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
    !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
    !define MUI_FINISHPAGE_SHOWREADME_TEXT "Make a small donation"
    !define MUI_FINISHPAGE_SHOWREADME "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3MXW2Z8VU3MJC"
    !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
    !define MUI_FINISHPAGE_LINK "NVFIX @ GitHub"
    !define MUI_FINISHPAGE_LINK_LOCATION "https://github.com/d-b/NVFIX"
    !define MUI_HEADERIMAGE
; *******************************************

; Pages
; *******************************************
    !define MUI_LICENSEPAGE_RADIOBUTTONS
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH
; *******************************************

; Languages
; *******************************************
    !insertmacro MUI_LANGUAGE "English"
; *******************************************

; Installation Sections
; *******************************************
    Function .onInit
        Call CheckPreviousInstance
        Call VerifyEnvironment
        Pop $0
        StrCmp $0 "0" 0 +2
            Abort
    FunctionEnd

    Function un.onInit
        Call un.CheckPreviousInstance
        Call un.VerifyEnvironment
        Pop $0
        StrCmp $0 "0" 0 +2
            Abort
    FunctionEnd

    Section "-NVFIX" NVFIX
        SectionIn RO
        SetOutPath $INSTDIR
        SetShellVarContext all
        Var /GLOBAL RETVAL

        nsExec::Exec 'net stop NVFIX'
        nsExec::Exec '"$INSTDIR\NVFIX.exe" uninstall'

        File "..\Release\NVFIX.exe"
        ClearErrors
        nsExec::Exec '"$INSTDIR\NVFIX.exe" install'
        Pop $RETVAL
        IfErrors +2 0
        StrCmp $RETVAL "0" +2 0
            Abort "Failed to install NVFIX as a service."
        nsExec::Exec 'net start NVFIX'

        CreateDirectory "$INSTDIR"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "DisplayName" "NVFIX: Clock Recovery Service"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "DisplayIcon" '"$INSTDIR\NVFIX.exe"'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "Publisher" 'Daniel Bloemendal'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "UninstallString" '"$INSTDIR\Uninstaller.exe"'
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "NoModify" 1
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX" "NoRepair" 1
        WriteUninstaller "$INSTDIR\Uninstaller.exe"

        CreateDirectory "$SMPROGRAMS\NVFIX\"
        CreateShortCut "$SMPROGRAMS\NVFIX\Uninstall.lnk" "$INSTDIR\Uninstaller.exe" "" "$INSTDIR\Uninstaller.exe" 0
        SetOutPath "$SMPROGRAMS\NVFIX\"
        File "Links\Make a small donation.url"
        File "Links\NVFIX @ GitHub.url"
    SectionEnd

    Section "Uninstall"
        SetShellVarContext all
        Delete "$INSTDIR\Uninstaller.exe"
        DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NVFIX"
        nsExec::Exec 'net stop NVFIX'
        nsExec::Exec '"$INSTDIR\NVFIX.exe" uninstall'
        Delete "$INSTDIR\NVFIX.exe"
        RMDir "$INSTDIR"
        Delete "$SMPROGRAMS\NVFIX\*.*"
        RMDir "$SMPROGRAMS\NVFIX"
    SectionEnd
; *******************************************
