; *******************************************
; *  Admin                                  *
; *******************************************

!macro macVerifyEnvironment
	ClearErrors
	UserInfo::GetName
	IfErrors Win9x
	Pop $0
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 NotAdmin
	Push "1"
	Goto Done

	NotAdmin:
		MessageBox MB_OK|MB_ICONEXCLAMATION "Administrative privileges are required for this operation!"
		Push "0"
		Goto Done

	Win9X:
		MessageBox MB_OK|MB_ICONEXCLAMATION "The Windows 9x family operating systems are not supported!"
		Push "0"
		Goto Done

	Done:
!macroend

Function VerifyEnvironment
	!insertmacro macVerifyEnvironment
FunctionEnd

Function un.VerifyEnvironment
	!insertmacro macVerifyEnvironment
FunctionEnd
