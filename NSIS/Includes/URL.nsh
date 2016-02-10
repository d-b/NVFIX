; *******************************************
; *  URL                                    *
; *******************************************

Function CreateInternetShortcut
    Exch $3
    Exch
    Exch $2
    Exch 3
    Exch $1
    Exch 4
    Exch $0
    WriteINIStr "$0.url" "InternetShortcut" "URL" "$1"
    WriteINIStr "$0.url" "InternetShortcut" "IconFile" "$2"
    WriteINIStr "$0.url" "InternetShortcut" "IconIndex" "$3"
    Pop $0
    Pop $3
    Pop $2
    Pop $1
FunctionEnd
