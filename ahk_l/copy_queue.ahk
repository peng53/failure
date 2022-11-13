#Requires AutoHotkey v1.1.33+
#NoEnv
ListLines Off
global MLINES := []
global MLINES_I := 1
^\::
	clipboad := ""
	sendinput, ^c
	ClipWait
	global MLINES := strsplit(trim(clipboard), "`n")
	global MLINES_I := 1
	MsgBox, % MLINES.MaxIndex() . " lines loaded."
	return
\::
	if (MLINES_I <= MLINES.MaxIndex()){
		sendinput, % MLINES[MLINES_I]
		MLINES_I++
	} else {
		MsgBox, "Queue is empty"
	}
	return
F9::
	if (MLINES_I > 1){
		MLINES_I--
	}
	return
F10::
	if (MLINES_I < MLINES.MaxIndex()){
		MLINES_I++
	}
	return
