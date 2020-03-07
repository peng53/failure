//var TimeEntry = {
//	starts, stops, diffs, codes
//};
var starts, stops, diffs, codes;
function getDiff(start, stop){
	let d1 = start.split(':',2).map(Number);
	let d2 = stop.split(':', 2).map(Number);
	let diff = d2[0]-d1[0];
	if (d1[0] > d2[0]){
		diff += 12;
	}
	diff += (d2[1]-d1[1])/60;
	return diff.toFixed(2);
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
function createTimesTable(start, rows){
	let timesdiv = document.getElementById('times');
	let table = document.createElement('table');
	table.innerHTML = "<tr><th>Code</th><th>Start</th><th>Stop</th><th>Hrs</th></tr>";
	for (let i=start, stop=rows+start; i<stop; ++i){
		let row = table.insertRow(-1);
		row.innerHTML = "<tr><td><input class='code' maxlength=3 /></td><td><input class='start' maxlength=5 /></td><td><input class='stop' maxlength=5 onblur=calc("+i+") /></td><td><input class='diff' maxlength=5 /></td></tr>";
	}
	timesdiv.appendChild(table);
}
function main(){
	createTimesTable(0,10);
	createTimesTable(10,10);
	
	diffs =  document.getElementsByClassName('diff');
	starts = document.getElementsByClassName('start');
	stops = document.getElementsByClassName('stop');
	codes = document.getElementsByClassName('code');
	
	let cnt = codes.length;
	for (let i=0; i<cnt; ++i){
		codes[i].value = i+100;
	}
}
