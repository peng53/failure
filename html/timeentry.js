var codesEnum = [];
var codesMap = {};
var treg = new RegExp(/\D/);
var codes, starts, stops, diffs;
function getDiff(start, stop){
	return (getDiffMinutes(start,stop)/60).toFixed(2);
}
function getDiffMinutes(start, stop){
	let d1 = start.split(treg,2).map(Number);
	let d2 = stop.split(treg, 2).map(Number);
	let diff = d2[0]-d1[0];
	if (d1[0] > d2[0]){
		diff += 12;
	}
	diff *= 60;
	diff += (d2[1]-d1[1]);
	return diff;
}
function calc(i){
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
function saya(event){
	calc(event.target['index']);
}
function createTimesTable(parent, rows){
	let start = ((codes === undefined)? 0 : codes.length);
	let table = document.createElement('table');
	table.innerHTML = "<thead><tr><th>Code</th><th>Start</th><th>Stop</th><th>Hrs</th></tr></thead>";
	let tbody = document.createElement('tbody');
	for (let i=start, stop=rows+start; i<stop; ++i){
		tbody.appendChild(createTimeRow(i));
	}
	table.appendChild(tbody);
	parent.appendChild(table);
	buildElTables();
	return table;
}

function createTimeRow(i){
	let row = document.createElement('tr');
	makeIntoInputCell(row.insertCell(0), 'code', 3);
	makeIntoInputCell(row.insertCell(1), 'start', 5);
	let stop = makeIntoInputCell(row.insertCell(2), 'stop', 5);
	stop['index'] = i;
	stop.addEventListener('blur', saya);
	makeIntoInputCell(row.insertCell(3), 'diff', 5);
	return row;
}

function makeIntoInputCell(cell, clss, length){
	let input = document.createElement('input');
	input.classList.add(clss);
	input.maxLength = length;
	cell.appendChild(input);
	return input;
}

function buildElTables(){
	codes = document.getElementsByClassName('code');
	starts = document.getElementsByClassName('start');
	stops = document.getElementsByClassName('stop');
	diffs = document.getElementsByClassName('diff');
}

function createTotalsTable(parent){
	let table = document.createElement('table');
	table.innerHTML = "<thead><tr><th>Code</th><th>Hrs</th></tr></thead>";
	let tbody = document.createElement('tbody');
	for (let i=0, l=codesEnum.length; i<l; ++i){
		tbody.appendChild(createTotalsRow(i,codesEnum[i]));
		codesMap[codesEnum[i]] = i;
	}
	table.appendChild(tbody);
	parent.appendChild(table);
}
function createTotalsRow(i, label){
	let row = document.createElement('tr');
	row.insertCell(0).appendChild(document.createTextNode(label));
	//makeIntoInputCell(row.insertCell(0), 'codee', 12).value = label;
	makeIntoInputCell(row.insertCell(1), 'total', 5).value = '';
	return row;
}

function rowIsValid(i){
	return (codes[i].value && starts[i].value && stops[i].value);
}

function totalUp(){
	let totals = {};
	let diff;
	for (let i=0,l=codes.length; i<l; ++i){
		if (rowIsValid(i)){
			diff = getDiffMinutes(starts[i].value, stops[i].value);
			if (diff > 0){
				if (!(codes[i].value in totals)){
					totals[codes[i].value] = 0;
				}
				totals[codes[i].value] += diff;
			}
		}
	}
	let totalRows = document.getElementsByClassName('total');
	for (key in totals){
		if (key in codesMap){
			totalRows[codesMap[key]].value = (totals[key]/60).toFixed(2);
		}
	}
}
