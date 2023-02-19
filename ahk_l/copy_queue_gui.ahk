#Requires AutoHotkey v1.1.33+
#NoEnv
ListLines Off

Gui, Test:New, +Resize,
Gui, Test:Default
Gui, Add, Button, Default w100 gFrmClip, From Clipboard
Gui, Add, Button, Default w100 x+5 yM gFrmFile, From File
Gui, Add, ListView, r20 w500 xM vMyListview, #|Line
Gui, Show, , Test

global MLINES := []
global MLINES_I := 1
global CSVMODE := 0

FrmClip:
	if (A_GuiEvent == "Normal"){
		Gui, Test:Default
		global MLINES := strsplit(trim(clipboard), "`n")
		global MLINES_I := 1
		GuiControl, -Redraw, MyListview
		LV_Delete()
		Loop % MLINES.MaxIndex() {
			LV_Add("",A_Index,MLINES[A_Index])
		}
		GuiControl, +Redraw, MyListview
		global CSVMODE := 0
		MsgBox, % MLINES.MaxIndex() . " lines loaded."
	}
return

FrmFile:
	if (A_GuiEvent == "Normal"){
		FileSelectFile, csvfile, , , Load a CSV, CSV File (*.csv)
		if (csvfile != ""){
			global MLINES :=[]
			global MLINES_I := 1
			Gui, Test:Default
			GuiControl, -Redraw, MyListview
			LV_Delete()
			Loop, read, % csvfile
			{
				t := []
				Loop, parse, A_Loopreadline, CSV
				{
					t.Push(A_Loopfield)
				}
				LV_Add("",A_Index,A_Loopreadline)
				MLINES.Push(t)
			}
			GuiControl, +Redraw, MyListview
			global CSVMODE := 1
		}
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

\::
	if (MLINES_I <= MLINES.MaxIndex()){
		if (CSVMODE == 1){
			l := MLINES[MLINES_I][1]
		} else {
			l := MLINES[MLINES_I]
		}
		sendinput, % l
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

guiclose:
Testguiclose:
	ExitApp