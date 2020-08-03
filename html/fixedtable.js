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

class TableBodyManip {
	constructor(tbody, rowenum, colenum, rowKey){
		this.tbody = tbody;
		this.rowenum = rowenum;
		this.colenum = colenum;
		this.rowKey = rowKey;

	}
	setRow(rowdata){
		const rn = this.rowenum.get(rowdata[this.rowKey]);
		const cols = this.tbody.querySelectorAll('tr')[rn].querySelectorAll('td');
		this.colenum.forEach(
			(val, key) => {
				if (key in rowdata) cols[val].textContent = rowdata[key];
			}
		);
		return this;
	}
}

