import { NCrossBoard } from './ncross.js';

function generateColorStyle(p,states,fontsize){
	let s = p.appendChild(document.createElement('style'));
	for (let i=0;i<states.length;++i){
		console.log(`.state${i} { fill: ${states[i]};}`);
		//s.sheet.addRule(`.state${i} { fill : ${states[i]};}`,0);
		s.textContent += `.state${i} { fill: ${states[i]}; color: ${states[i]};}`;
	}
	s.textContent += `text { font-size: ${fontsize}%}`;
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
		// need to remove tiles if already there
		generateColorStyle(this.svg,this.colorStates,this.metrics.hintReserve);
		console.log(this.board);
		for (let r=0;r<this.board.height;++r){
			for (let c=0;c<this.board.width;++c){
				this.createTile(r,c);
			}
		}
		for (let r=0;r<this.board.height;++r){
			this.createRHint(r);
		}
		for (let c=0;c<this.board.width;++c){
			this.createCHint(c);
		}
	}
	createTile(r,c){
		let tile = this.svg.appendChild(
			document.createElementNS("http://www.w3.org/2000/svg", 'rect'));
		tile.setAttributeNS(null, 'x', this.metrics.hintReserve+c*this.metrics.twidth);
		tile.setAttributeNS(null, 'y', this.metrics.hintReserve+r*this.metrics.theight);
		tile.setAttributeNS(null, 'width', this.metrics.twidth);
		tile.setAttributeNS(null, 'height', this.metrics.theight);
		tile.classList.add(`state${this.board.A[r][c]}`);
	}
	createRHint(r){
		let text;
		let twidth = this.metrics.hintReserve/this.board.rowHints[r].length;
		for (let i=0;i<this.board.rowHints[r].length;++i){
			text = this.svg.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.rowHints[r][i].cnt));
			text.setAttribute('x', twidth*i);
			text.setAttribute('y', 1.25*this.metrics.hintReserve+r*this.metrics.theight);
			text.classList.add(`state${this.board.rowHints[r][i].state}`);
		}
	}
	createCHint(c){
		let text;
		let theight = this.metrics.hintReserve/(this.board.colHints[c].length+1);
		for (let i=0;i<this.board.colHints[c].length;++i){
			text = this.svg.appendChild(
				document.createElementNS("http://www.w3.org/2000/svg", 'text'));
			text.appendChild(document.createTextNode(this.board.colHints[c][i].cnt));
			text.setAttribute('y', theight*(i+1));
			text.setAttribute('x', 1.25*this.metrics.hintReserve+c*this.metrics.twidth);
			text.classList.add(`state${this.board.colHints[c][i].state}`);
		}
	}
}