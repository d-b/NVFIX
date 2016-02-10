; *******************************************
; *  Mutex                                  *
; *******************************************

!macro macCheckPreviousInstance
	;Check if already running
	;If so don't open another but bring to front
	System::Call "kernel32::CreateMutexA(i 0, i 0, t '$(^Name)') i .r0 ?e"
	Pop $0
	StrCmp $0 0 launch
	StrLen $0 "$(^Name)"
	IntOp $0 $0 + 1

	loop:
		FindWindow $1 '#32770' '' 0 $1
		IntCmp $1 0 +4
		System::Call "user32::GetWindowText(i r1, t .r2, i r0) i."
		StrCmp $2 "$(^Name)" 0 loop
		System::Call "user32::ShowWindow(i r1,i 9) i."         ; If minimized then maximize
		System::Call "user32::SetForegroundWindow(i r1) i."    ; Bring to front
		Abort
	launch:
!macroend

Function CheckPreviousInstance
	!insertmacro macCheckPreviousInstance
FunctionEnd

Function un.CheckPreviousInstance
	!insertmacro macCheckPreviousInstance
FunctionEnd