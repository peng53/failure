'use strict';

function RampCols (cols){
	this.cur = 0
	this.cols = new Array(cols);
	for (let i=0;i<cols;++i){
		this.cols[i] = [];
	}
}
RampCols.prototype.ins = function(v){
	if (this.cur==this.cols.length || 
		this.cur!=0 && this.cols[this.cur].length+1 == this.cols[this.cur-1].length){
		// if not first column AND new count will equal previous column's current count
		this.cur = 0;
	}
	const i = this.cur;
	const j = this.cols[this.cur].length;
	this.cols[this.cur].push(v);
	++this.cur;
	return [i,j];
}

RampCols.prototype.renderAsTable = function(){
	const ret = document.createElement('table');
	let row, cell;
	for (let r=0;r<this.cols.length;++r){
		row = ret.insertRow(-1);
		for (let c= 0, l=this.cols[r].length;c<l;++c){
			cell = row.insertCell(-1);
			cell.appendChild(document.createTextNode(this.cols[r][c]));
		}
	}
	return ret;
}