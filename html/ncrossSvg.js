import { NCrossBoard } from './ncross.js';

function generateColorStyle(p,states){
	let s = p.appendChild(document.createElement('style'));
	for (let i=0;i<states.length;++i){
		console.log(`.state${i} { fill: ${states[i]};}`);
		//s.sheet.addRule(`.state${i} { fill : ${states[i]};}`,0);
		s.textContent += `.state${i} { fill: ${states[i]};}`;
	}
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
		generateColorStyle(this.svg,this.colorStates);
		console.log(this.board);
		for (let r=0;r<this.board.height;++r){
			for (let c=0;c<this.board.width;++c){
				this.createTile(r,c);
			}
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
}