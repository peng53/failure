function getRandomIntInclusive(min: number, max: number) : number {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(Math.random() * (max - min + 1) + min); //The maximum is inclusive and the minimum is inclusive
}

export class Hint {
	state: number;
	cnt: number;
	constructor(state: number, cnt: number){
		this.state = state;
		this.cnt = cnt;
	}
	inc(): void {
		++this.cnt;
	}
}

export class NCrossBoard {
	width: number;
	height: number;
	states: string[];
	A: Uint8Array[];
	rowHints: Hint[][];
	colHints: Hint[][];
	constructor(width: number, height:number ,states: string[]){
		this.width = width;
		this.height = height;
		this.states = states;
		this.clear();
	}
	clear(): void {
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
	recalcRow(r: number): void {
		let hint: Hint[] = [];
		for (let c: number=0;c<this.width;++c){
			if (this.A[r][c]!=0){ // if this tile is filled..
				if (c>0 && this.A[r][c-1]==this.A[r][c]){ // and the previous adjacent one was filled
					// then use the most current 'hint bar'
					hint[hint.length-1].inc();
				} else { // otherwise create a new one
					hint.push(new Hint(this.A[r][c],1));
				}
			}
		}
		this.rowHints[r] = hint;
	}
	recalcCol(c: number): void {
		let hint: Hint[] = [];
		for (let r: number=0;r<this.height;++r){
			if (this.A[r][c]!=0){ // if this tile is filled..
				if (r>0 && this.A[r-1][c]==this.A[r][c]){ // and the previous adjacent one was filled with same color
					// then use the most current 'hint bar'
					hint[hint.length-1].inc();
				} else { // otherwise create a new one
					hint.push(new Hint(this.A[r][c],1));
				}
			}
		}
		this.colHints[c] = hint;
	}
	generate(): void {
		for (let r: number=0;r<this.height;++r){
			for (let c: number=0;c<this.width;++c){
				this.A[r][c] = getRandomIntInclusive(0,this.states-1);
			}
			this.recalcRow(r);
		}
		for (let c: number=0;c<this.width;++c){
			this.recalcCol(c);
		}
	}
	cycleNext(r: number,c: number): number {
		this.A[r][c] = (this.A[r][c]+1)%this.states;
		return this.A[r][c];
	}
}