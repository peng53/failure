#Requires AutoHotkey v1.1.33+
#NoEnv
ListLines Off
Gui, Test:New, +Resize,
Gui, Test:Add, ListView, r20 w500, #|Line
Gui, Test:Show, , Test

#IfWinActive ahk_class AutoHotkeyGUI
	a::
		Gui, Test:Default
	;	MsgBox, Hello
		LV_Add("",1,"Test")
		return
#If

return
GuiClose:
	ExitApp