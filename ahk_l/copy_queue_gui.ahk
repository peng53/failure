#Requires AutoHotkey v1.1.33+
#NoEnv
ListLines Off

Gui, Test:New, +Resize,
Gui, Test:Add, Button, Default w100 gFrmClip, From Clipboard
Gui, Test:Add, ListView, r20 w500, #|Line
Gui, Test:Show, , Test

FrmClip:
	if (A_GuiEvent == "Normal"){
	Gui, Test:Default
	global MLINES := strsplit(trim(clipboard), "`n")
	global MLINES_I := 1
	Gui, Test:Default
	LV_Delete()
	Loop % MLINES.MaxIndex() {
		LV_Add("",A_Index,MLINES[A_Index])
	}
	MsgBox, % MLINES.MaxIndex() . " lines loaded."
	}
return

#IfWinActive ahk_class AutoHotkeyGUI
	Esc::
		Gui, Test:Default
		global MLINES := []
		global MLINES_I := 1
		LV_Delete()
		return
#If

global MLINES := []
global MLINES_I := 1
\::
	if (MLINES_I <= MLINES.MaxIndex()){
		sendinput, % MLINES[MLINES_I]
		MLINES_I++
	} else {
		MsgBox, Queue is empty
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

return
GuiClose:
	ExitApp
