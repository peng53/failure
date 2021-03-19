import { NCrossBoard } from './ncross.js';

function generateColorStyle(p: HTMLElement,states: string[],fontsize: number): void {
	let s = p.appendChild(document.createElement('style'));
	for (let i: number=0;i<states.length;++i){
		//s.sheet.addRule(`.state${i} { fill : ${states[i]};}`,0);
		s.textContent += `.state${i} { fill: ${states[i]}; color: ${states[i]};}`;
		//s.textContent += `rect[data-state="${i}"] { fill: ${states[i]}; color: ${states[i]};}`;
	}
	s.textContent += `text { font-size: ${fontsize}%; } rect { stroke: #eee; stroke-width: 1; }`;
}



export class NCrossSvg {
	svg: HTMLElement;
	colorStates: string[];
	board: NCrossBoard;
	metrics: {
		hintReserve: number
		twidth: number
		theight: number
	}
	constructor(svg: HTMLElement,r: number,c: number,states: string[]){
		this.svg = svg;
		this.board = new NCrossBoard(r,c,states.length);
		this.metrics = {
			hintReserve : 15
		,	twidth : (100-15)/c
		,	theight : (100-15)/r
		};
		this.colorStates = states;
	}
	generate(): void {
		this.board.generate();
		this.init();
	}
	init(): void {
		generateColorStyle(this.svg,this.colorStates,this.metrics.hintReserve);
		const tthis: NCrossSvg = this;
		const tileF = function(event){
			tthis.clickedRecalc(event.target);
		};
		const s: DocumentFragment = new DocumentFragment();
		for (let r: number=0;r<this.board.height;++r){
			for (let c: number=0;c<this.board.width;++c){
				this.createTile(s,r,c).addEventListener('click',tileF);
			}
		}
		for (let r: number=0;r<this.board.height;++r){
			this.createRHint(s,r);
		}
		for (let c: number=0;c<this.board.width;++c){
			this.createCHint(s,c);
		}
		this.svg.appendChild(s);
	}
	createTile(s: DocumentFragment,r: number,c: number): SVGRectElement {
		let tile: SVGRectElement = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'rect'));
		tile.setAttributeNS(null, 'x', (this.metrics.hintReserve+c*this.metrics.twidth).toString());
		tile.setAttributeNS(null, 'y', (this.metrics.hintReserve+r*this.metrics.theight).toString());
		tile.setAttributeNS(null, 'width', (this.metrics.twidth).toString());
		tile.setAttributeNS(null, 'height', (this.metrics.theight).toString());
		tile.classList.add(`state${this.board.A[r][c]}`,`row${r}col${c}`);
		tile.dataset['state']=this.board.A[r][c].toString();
		tile.dataset['row']=r.toString();
		tile.dataset['col']=c.toString();
		return tile;
	}
	createRHint(s: DocumentFragment | HTMLElement,r: number){
		let g: SVGGElement = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'g'));
		g.classList.add(`hintr${r}`);
		let text: SVGTextElement;
		let twidth: number = this.metrics.hintReserve/this.board.rowHints[r].length;
		for (let i: number=0;i<this.board.rowHints[r].length;++i){
			text = g.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.rowHints[r][i].cnt.toString()));
			text.setAttribute('x', (twidth*i).toString());
			text.setAttribute('y', (1.25*this.metrics.hintReserve+r*this.metrics.theight).toString());
			text.classList.add(`state${this.board.rowHints[r][i].state}`);
		}
	}
	createCHint(s: DocumentFragment | HTMLElement,c: number){
		let g: SVGGElement = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'g'));
		g.classList.add(`hintc${c}`);
		let text: SVGTextElement;
		let theight: number = this.metrics.hintReserve/(this.board.colHints[c].length+1);
		for (let i: number=0;i<this.board.colHints[c].length;++i){
			text = g.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.colHints[c][i].cnt.toString()));
			text.setAttribute('y', (theight*(i+1)).toString());
			text.setAttribute('x', (1.25*this.metrics.hintReserve+c*this.metrics.twidth).toString());
			text.classList.add(`state${this.board.colHints[c][i].state}`);
		}
	}
	clickedRecalc(tile: HTMLElement): void{
		const r: number = parseInt(tile.dataset.row), c: number = parseInt(tile.dataset.col);
		const os: number = this.board.A[r][c], ns: number = this.board.cycleNext(r, c);
		tile.classList.remove(`state${os}`);
		tile.classList.add(`state${ns}`);
		this.board.recalcRow(r);
		this.board.recalcCol(c);
		this.svg.removeChild(this.svg.querySelector(`.hintr${r}`));
		this.svg.removeChild(this.svg.querySelector(`.hintc${c}`));
		this.createRHint(this.svg,r);
		this.createCHint(this.svg,c);
	}
	recalMetrics(): void {
		this.metrics.twidth = (100-this.metrics.hintReserve)/this.board.width;
		this.metrics.theight = (100-this.metrics.hintReserve)/this.board.height;
		this.svg.textContent ='';
		this.init();
	}
	set hintReserve(p: number){
		if (p!=this.metrics.hintReserve){
			this.metrics.hintReserve = p;
			this.recalMetrics();
		}
	}
	resizeBoard(r: number,c: number): void {
		if (r!=this.board.height || c!=this.board.width){
			this.board.width = c;
			this.board.height = r;
			this.board.clear();
		}
		this.board.generate();
		this.recalMetrics();
	}
}