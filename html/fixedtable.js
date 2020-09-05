'use strict';
function tableBodyDom(rowenum, cols){
	const ele = document.createElement('tbody');
	rowenum.forEach(
		(val, key) => {
			const row = ele.insertRow(-1);
			for (let i=0; i<=cols; ++i){
				row.insertCell(-1).appendChild(document.createTextNode(''));
			}
			row.querySelector('td').textContent = key;
		}
	);
	return ele;
}
function tableHeadDom(cols){
	// Where cols is an ordered array of col headings
	const ele = document.createElement('thead');
	const row = ele.insertRow(-1);
	for (const v of cols){
		row.appendChild(document.createElement('th')).appendChild(document.createTextNode(v));
	}
	return ele;
}


class TableBodyManip {
	constructor(tbody, rowenum, colenum, rowKey){
		this.tbody = tbody;
		this.rowenum = rowenum;
		this.colenum = colenum;
		this.rowKey = rowKey;

	}
	setRow(rowdata){
		if (this.rowKey in rowdata && this.rowenum.has(rowdata[this.rowKey])){
			const rn = this.rowenum.get(rowdata[this.rowKey]);
			const cols = this.tbody.querySelectorAll('tr')[rn].querySelectorAll('td');
			this.colenum.forEach(
				(val, key) => {
					if (val != 0 && key in rowdata) cols[val].textContent = rowdata[key];
				}
			);
			return this;
		}
	}
}

