function getRandomIntInclusive(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(Math.random() * (max - min + 1) + min); //The maximum is inclusive and the minimum is inclusive
}

export class NCrossBoard {
	constructor(width, height){
		this.width = width;
		this.height = height;
		this.clear();
	}
	clear(){
		this.A = [];
		this.rowHints = [];
		for (let r=0;r<this.height;++r){
			this.A[r] = new Uint8Array(this.width);
			this.A[r].fill(0);
			this.rowHints.push([]);
		}
		this.colHints = [];
		for (let c=0;c<this.width;++c){
			this.colHints.push([]);
		}
	}
	recalcRow(r){
		let hint = [];
		for (let c=0;c<this.width;++c){
			if (this.A[r][c]==1){ // if this tile is filled..
				if (c>0 && this.A[r][c-1]==1){ // and the previous adjacent one was filled
					// then use the most current 'hint bar'
					hint[hint.length-1] += 1;
				} else { // otherwise create a new one
					hint.push(1);
				}
			}
		}
		this.rowHints[r] = hint;
	}
	recalcCol(c){
		let hint = [];
		for (let r=0;r<this.height;++r){
			if (this.A[r][c]==1){ // if this tile is filled..
				if (r>0 && this.A[r-1][c]==1){ // and the previous adjacent one was filled
					// then use the most current 'hint bar'
					hint[hint.length-1] += 1;
				} else { // otherwise create a new one
					hint.push(1);
				}
			}
		}
		this.colHints[c] = hint;
	}
	generate(){
		for (let r=0;r<this.height;++r){
			for (let c=0;c<this.width;++c){
				this.A[r][c] = getRandomIntInclusive(0,1);
			}
			this.recalcRow(r);
		}
		for (let c=0;c<this.width;++c){
			this.recalcCol(c);
		}
	}
}