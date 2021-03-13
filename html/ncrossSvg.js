import { NCrossBoard } from './ncross.js';

function generateColorStyle(p,states,fontsize){
	let s = p.appendChild(document.createElement('style'));
	for (let i=0;i<states.length;++i){
		//s.sheet.addRule(`.state${i} { fill : ${states[i]};}`,0);
		s.textContent += `.state${i} { fill: ${states[i]}; color: ${states[i]};}`;
	}
	s.textContent += `text { font-size: ${fontsize}%; } rect { stroke: #eee; stroke-width: 1; }`;
}

export class NCrossSvg {
	constructor(svg,r,c,states){
		this.svg = svg;
		this.board = new NCrossBoard(r,c,states.length);
		this.metrics = {
			hintReserve : 15
		,	twidth : (100-15)/c
		,	theight : (100-15)/r
		};
		this.colorStates = states;
	}
	generate(){
		this.board.generate();
		this.init();
	}
	init(){
		generateColorStyle(this.svg,this.colorStates,this.metrics.hintReserve);
		const tthis = this;
		const tileF = function(event){
			tthis.clickedRecalc(event.target);
		};
		const s = new DocumentFragment();
		for (let r=0;r<this.board.height;++r){
			for (let c=0;c<this.board.width;++c){
				this.createTile(s,r,c).addEventListener('click',tileF);
			}
		}
		for (let r=0;r<this.board.height;++r){
			this.createRHint(s,r);
		}
		for (let c=0;c<this.board.width;++c){
			this.createCHint(s,c);
		}
		this.svg.appendChild(s);
	}
	createTile(s,r,c){
		let tile = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'rect'));
		tile.setAttributeNS(null, 'x', this.metrics.hintReserve+c*this.metrics.twidth);
		tile.setAttributeNS(null, 'y', this.metrics.hintReserve+r*this.metrics.theight);
		tile.setAttributeNS(null, 'width', this.metrics.twidth);
		tile.setAttributeNS(null, 'height', this.metrics.theight);
		tile.classList.add(`state${this.board.A[r][c]}`,`row${r}col${c}`);
		tile.dataset['row']=r;
		tile.dataset['col']=c;
		return tile;
	}
	createRHint(s,r){
		let g = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'g'));
		g.classList.add(`hintr${r}`);
		let text;
		let twidth = this.metrics.hintReserve/this.board.rowHints[r].length;
		for (let i=0;i<this.board.rowHints[r].length;++i){
			text = g.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.rowHints[r][i].cnt));
			text.setAttribute('x', twidth*i);
			text.setAttribute('y', 1.25*this.metrics.hintReserve+r*this.metrics.theight);
			text.classList.add(`state${this.board.rowHints[r][i].state}`);
		}
	}
	createCHint(s,c){
		let g = s.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'g'));
		g.classList.add(`hintc${c}`);
		let text;
		let theight = this.metrics.hintReserve/(this.board.colHints[c].length+1);
		for (let i=0;i<this.board.colHints[c].length;++i){
			text = g.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.colHints[c][i].cnt));
			text.setAttribute('y', theight*(i+1));
			text.setAttribute('x', 1.25*this.metrics.hintReserve+c*this.metrics.twidth);
			text.classList.add(`state${this.board.colHints[c][i].state}`);
		}
	}
	clickedRecalc(tile){
		const r = parseInt(tile.dataset.row), c = parseInt(tile.dataset.col);
		const os = this.board.A[r][c], ns = this.board.cycleNext(r, c);
		tile.classList.remove(`state${os}`);
		tile.classList.add(`state${ns}`);
		this.board.recalcRow(r);
		this.board.recalcCol(c);
		this.svg.removeChild(this.svg.querySelector(`.hintr${r}`));
		this.svg.removeChild(this.svg.querySelector(`.hintc${c}`));
		this.createRHint(this.svg,r);
		this.createCHint(this.svg,c);
	}
	recalMetrics(){
		this.metrics.twidth = (100-this.metrics.hintReserve)/this.board.width;
		this.metrics.theight = (100-this.metrics.hintReserve)/this.board.height;
		this.svg.textContent ='';
		this.init();
	}
	set hintReserve(p){
		if (p!=this.metrics.hintReserve){
			this.metrics.hintReserve = p;
			this.recalMetrics();
		}
	}
	resizeBoard(r,c){
		if (r!=this.board.height || c!=this.board.width){
			this.board.width = c;
			this.board.height = r;
			this.board.clear();
		}
		this.board.generate();
		this.recalMetrics();
	}
}