#Requires AutoHotkey v2.0

App := Gui('+Resize +MinSize300x225')
Wins := App.Add('listview','NoSortHdr Grid w280 h180 Checked',['Area','X','Y','W','H'])
Wins.OnEvent('ItemCheck',UpdatePos)
Wins.OnEvent('ItemFocus',DemoWin)
LoadButton := App.Add('button',,'Load')
LoadButton.OnEvent('Click',LoadPos)
SaveButton := App.Add('button','xs+50 yp','Save')
SaveButton.OnEvent('Click',SavePos)
OnTopBox := App.Add('CheckBox', 'xs+100 yp+5' , 'OnTop')
OnTopBox.OnEvent('Click',ToggleTopApp)

ToggleTopApp(thisgui,infos){
	WinSetAlwaysOnTop(OnTopBox.Value,App)
}

UpdatePos(thisgui,rowChecked,checked){
	if (checked == 1){
		WinGetPos(&x, &y, &w, &h, App)
		Wins.Modify(rowChecked, '-Check', Wins.GetText(rowChecked,1), x, y, w, h)
	}
}
SaveActive(n){
	WinGetPos(&x, &y, &w, &h, 'A')
	Wins.Modify(n, '', Wins.GetText(n,1), x, y, w, h)
}
DemoWin(thisgui,rowClicked){
	x := Wins.GetText(rowClicked, 2)
	y := Wins.GetText(rowClicked, 3)
	w := Wins.GetText(rowClicked, 4)
	h := Wins.GetText(rowClicked, 5)
	Wins.Modify(rowClicked, '-Focus')
	WinMove(x,y,w,h,App)
}

WinRow := Map()


Wins.ModifyCol(1, 70)
Loop 4 {
	Wins.ModifyCol(A_Index+1, 50)
}

ActivePos(char){
	rowNeeded := WinRow.Get(char,1)
	x := Wins.GetText(rowNeeded, 2)
	y := Wins.GetText(rowNeeded, 3)
	w := Wins.GetText(rowNeeded, 4)
	h := Wins.GetText(rowNeeded, 5)
	WinMove(x,y,w,h,'A')
}

for char in ['1','2','3','4','A']  {
	Wins.Add(, char, (A_Index-1)*400, 0, 800, A_ScreenHeight-32)
	WinRow.Set(char,A_Index)
}

LoadPos(thisgui,infos){
	inifile := A_ScriptDir . '\win_position.ini'
	Wins.Delete()
	WinRow.Clear()
	allsections := IniRead(inifile, 'Sections', 'Areas')
	Loop Parse allsections, '|'{
		if (StrLen(A_LoopField)>0){
			x := IniRead(inifile, A_LoopField, 'x')
			y := IniRead(inifile, A_LoopField, 'y')
			w := IniRead(inifile, A_LoopField, 'w')
			h := IniRead(inifile, A_LoopField, 'h')
			Wins.Add(, A_LoopField, x, y, w, h)
			WinRow.Set(A_LoopField,A_Index)
		}
	}
}

SavePos(thisgui,infos){
	inifile := A_ScriptDir . '\win_position.ini'
	allsections := ''
	for char,row in WinRow {
		allsections .= char . '|'
		x := Wins.GetText(row, 2)
		y := Wins.GetText(row, 3)
		w := Wins.GetText(row, 4)
		h := Wins.GetText(row, 5)
		IniWrite x, inifile, char, 'x'
		IniWrite y, inifile, char, 'y'
		IniWrite w, inifile, char, 'w'
		IniWrite h, inifile, char, 'h'
	}
	IniWrite allsections, inifile, 'Sections', 'Areas'
}

App.Show('w300 h225')

#HotIf WinActive(App)

#HotIf

!1:: ActivePos('1')
!2:: ActivePos('2')
!3:: ActivePos('3')
!4:: ActivePos('4')
!A:: ActivePos('A')

!+1::SaveActive('1')
!+2::SaveActive('2')
!+3::SaveActive('3')
!+4::SaveActive('4')
!+A::SaveActive('A')
