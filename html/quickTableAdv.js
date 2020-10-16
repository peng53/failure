function tableAddCol(text){
	addTh(text);
	Array.from(document.querySelectorAll('tbody tr')).forEach(function(r){r.insertCell(-1);});
}
function nullStringArr(cnt){
	const arr = [];
	for (let i=0;i<cnt;++i) arr.push('');
	return arr;
}
function tableAddRow(){
	addTr(nullStringArr(document.querySelectorAll('#header th').length));
}
function ArrayFrum(xs){
	let r = [];
	for (let i=0,l=xs.length;i<l;++i){
		r.push(xs[i]);
	}
	return r;
}

function pageData(){
	const colNames = Array.from(document.querySelectorAll('#header th')).map(function(x){return x.textContent;});
	const rowData = Array.from(document.querySelectorAll('tbody tr')).
		map(function(r){
			return Array.from(r.querySelectorAll('td')).
				map(function(x){return x.textContent;});
		});
	return {cols:colNames, data:rowData, title:titlecell.textContent};
}
function exportBlob(){
	const blob=new Blob([JSON.stringify(pageData())], {type:'text/json'});
	window.open(URL.createObjectURL(blob));
}
function page2qs(){
	const data = pageData();
	const rows = data.data.map(function(r){return r.join('|');});
	const parts = ['title='+data.title, 'datasep=row', 'cols='+data.cols.join(':'), 'rows='+rows.join(':')];
	prompt('Query string is:', '?'+parts.join('&'));
}

function controlPanel(){
	let d = document.body.appendChild(document.createElement('div'));
	d.classList.add('controlpanel');
	const addButton = function(p, t, f){
		const b = p.appendChild(document.createElement('button'));
		b.textContent = t;
		b.onclick = f;
	};
	addButton(d, 'Add Column', function(){
		const text=prompt('Column name:');
		if (text) tableAddCol(text);
	});
	addButton(d, 'Add Row', tableAddRow);
	addButton(d, 'Save', exportBlob);
	addButton(d, 'Edit', function(){
		const t = document.querySelector('table');
		t.contentEditable = (t.contentEditable=='true') ? 'false' : 'true';
	});
	addButton(d, 'Edit Cell', editCellStart);
	return d;
}

function editCell(e){
	const cell = e.target;
	if (cell.tagName=='TD' || cell.tagName=='TH'){
		const text = prompt('New text:', cell.textContent);
		if (text!==null) cell.textContent = text;
		document.querySelector('table').onclick = undefined;
	}
}
function editCellStart(){
	document.querySelector('table').onclick = editCell;
}

const cp = controlPanel();
addEventListener('keydown', function(e){
	if (e.key=='Control'){
		cp.classList.toggle('hidden');
	}
});
