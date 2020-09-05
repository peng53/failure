'use strict';
function formatNotes(data){
	const lines = data;
	const outv = [];
	const codes = {'Prod' : 0};

	let ts, te, code, out, diff;
	for (let i=0,m=lines.length;i<m;i+=4){
		// lines[i+3] is ignored as an empty line
		if (lines[i].length === 0 || lines[i+1].length === 0 || lines[i+2].length === 0){
			console.log('Invalid at line = '+i);
			break;
		}
		ts = new Date(Date.parse(lines[i]));
		if (te){
			diff = (ts - te)/60000;
			if (diff > 0){
				codes['Prod'] += diff;
				outv.push({code: ' ', start: niceTime(te,':'), end: niceTime(ts,':'), total: (diff/60).toFixed(2)});
			}
		}
		code = lines[i+1];
		te = new Date(Date.parse(lines[i+2]));
		diff = (te - ts)/60000;
		if (diff > 0){
			if (!(code in codes)){
				codes[code] = 0;
			}
			codes[code] += diff;
			outv.push({code:code, start: niceTime(ts,':'), end: niceTime(te,':'), total: (diff/60).toFixed(2)});
		}
	}
	return [outv, codes];
}
function niceTime(td, sep){
	let out = ''
	if (td.getHours()<10){
		out += ' ';
	}
	out += td.getHours();
	out += sep;
	if (td.getMinutes()<10){
		out += '0';
	}
	out += td.getMinutes();
	return out;
}
function addTimeRow(table, data){
	const row = table.querySelector('tbody').insertRow(-1);
	row.insertCell(-1).appendChild(document.createTextNode(data.code));
	row.insertCell(-1).appendChild(document.createTextNode(data.start));
	row.insertCell(-1).appendChild(document.createTextNode(data.end));
	row.insertCell(-1).appendChild(document.createTextNode(data.total));
	return row;
}

function addDataToTables(tables, data, rowlimit, f){
	let i=0;
	for (let t=0; t<tables.length; ++t){
		for (;i<data.length && i<rowlimit; ++i){
			f(tables[t], data[i]);
		}
		rowlimit *= 2;
	}
}

function addCodes(table, codes){
	const body = table.querySelector('tbody');
	let row;
	for (let key in codes){
		row = body.insertRow(-1);
		row.insertCell(-1).textContent = key;
		row.insertCell(-1).textContent = (codes[key]/60).toFixed(2);
	}
}

function parseDataToBoxes(data, timerowTables, rowLimit, totalTable){
	const formattedNotes = formatNotes(data);
	const parsedData = formattedNotes[0], codes = formattedNotes[1];
	addDataToTables(timerowTables, parsedData, rowLimit, addTimeRow);
	addCodes(totalTable, codes);
	for (let i=0,l=timerowTables.length;i<l;++i){
		fillTable(timerowTables[i], rowLimit);
	}
}

function setUpInputBox(){
	const div = document.createElement('div');
	div.style.cssText = 'position: absolute; bottom: 0; width: 100vw; background: beige;';
	div.classList.add('hidden');

	let tb = div.appendChild(document.createElement('textarea'));
	tb.style.cssText = 'width: 100%; height: 100%;';

	tb.value = '2:00 PM 8/22/2020\n123\n2:05 PM 8/22/2020\n\n2:07 PM 8/22/2020\n134\n2:10 PM 8/22/2020';
	div.appendChild(document.createElement('br'));

	let gobutton = div.appendChild(document.createElement('button'));
	gobutton.textContent = 'Generate';
	gobutton.onclick = function(){
		parseDataToBoxes(
			div.querySelector('textarea').value.split('\n'),
			rows.querySelectorAll('table'),
			22,
			totals.querySelector('table')
		);
		div.parentNode.removeChild(div);
	};
	let xbutton = div.appendChild(document.createElement('button'));
	xbutton.textContent = 'X';
	xbutton.onclick = function(){ div.remove();};

	document.body.appendChild(div);
}


function fillTable(table, limit){
	const tbody = table.querySelector('tbody');
	let n = tbody.children.length;
	const blank = {code:'',start:'',end:'',total:''};
	while (n<limit){
		addTimeRow(table, blank);
		++n;
	}
}
setUpInputBox();
