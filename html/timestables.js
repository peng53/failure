var rowCount = 0;
var treg = new RegExp(/\D/);
var codes, starts, stops, diffs;
function buildElTables(){
	codes = document.getElementsByClassName('code');
	starts = document.getElementsByClassName('start');
	stops = document.getElementsByClassName('stop');
	diffs = document.getElementsByClassName('diff');
}
function getDiff(start, stop){
	let d1 = start.split(treg,2).map(Number);
	let d2 = stop.split(treg, 2).map(Number);
	let diff = d2[0]-d1[0];
	if (d1[0] > d2[0]){
		diff += 12;
	}
	diff += (d2[1]-d1[1])/60;
	return diff.toFixed(2);
}

function calc(event){
	let i = event.target['index'];

	if (starts[i].value.length == 0 || stops[i].value.length == 0 ){
		return;
	}
	diffs[i].value = getDiff(starts[i].value, stops[i].value);
	if (i<2 || starts[i-1].value.length != 0 || stops[i-2].value.length == 0){
		return;
	}
	// Attempt to find inbetween row above
	stops[i-1].value = starts[i].value;
	starts[i-1].value = stops[i-2].value;
	diffs[i-1].value = getDiff(starts[i-1].value, stops[i-1].value);
	codes[i-1].value = 'ATO';
}
function timemac(event){
	let ele = event.target;
	switch (event.key){
		case 'n':
			ele.value = 'tbd';
			break;
		case 'c':
			ele.value = '';
			break;
		default:
			break;
	}
}

function addRows(count){
	let times = document.getElementById('times');
	let row;
	for (let i=0; i<count; ++i){
		addRow(times, rowCount);
		++rowCount;
	}
	buildElTables();
	let entry = document.getElementById('entry');
	entry.scrollTop = entry.scrollHeight;
}
function addRow(table, index){
	let row = table.insertRow(-1);
	addInputCell(row, 0, 'code', 4, index).value = rowCount;
	addInputCell(row, 1, 'start', 5, index).addEventListener('keyup', timemac);
	let stop = addInputCell(row, 2, 'stop', 5, index);
	stop.addEventListener('blur', calc);
	stop.addEventListener('keyup', timemac);
	addInputCell(row, 3, 'diff', 5, index);
}
function addInputCell(row, col, clss, maxlength, index){
	let cell = row.insertCell(col);
	let input = document.createElement('input');
	input.classList.add(clss);
	input.maxLength = maxlength;
	input['index'] = index;
	cell.appendChild(input);
	return input;
}
function rowHasData(i){
	return (starts[i].value && stops[i].value && diffs[i].value && codes[i].value);
}
function getFormatedRow(i){
	return "ROW";
}
function exportRows(){
	let out = document.getElementById('otd');
	for (let i=0; i<rowCount; ++i){
		if (rowHasData(i)){
			out.value += getFormatedRow(i);
			out.value += '\n';
		}
	}
}
