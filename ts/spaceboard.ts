import { Entity } from './mv_entity.js';

class Space {
	color : {
		r : number
		g : number
		b : number
	}
	constructor(color : {r: number, g: number, b: number}){
		this.color = color;
	}
}

const colorWhite = {r: 255, g: 255, b: 255};

class EntityAndPos {
	entity : Entity
	pos : { x: number, y: number}
	constructor(entity: Entity, pos: {x: number, y: number}){
		this.entity = entity;
		this.pos = pos;
	}
}

class Board {
	adj : Space[][]
	width: number
	height: number
	entities: EntityAndPos[]
	constructor(width: number, height: number){
		this.width = width;
		this.height = height;
		this.adj = new Array(height);
		for (let i=0;i<height;++i){
			this.adj[i] = new Array(width);
			for (let j=0; j<width; ++j){
				this.adj[i][j] = new Space(colorWhite);
			}
		}
	}
	turn() : void {
		let dir : string;
		let mag : number;
		for (let enp of this.entities){
			if (enp.entity.leMod){
				this.adj[enp.pos.y][enp.pos.x].color = enp.entity.leMod.mod(this.adj[enp.pos.y][enp.pos.x].color);
			}
			[dir,mag] = enp.entity.rng();
			switch (dir){
				case 'y':
					enp.pos.y += mag;
					if (enp.pos.y>=this.height){
						enp.pos.y %= this.height;
					} else if (enp.pos.y < 0){
						enp.pos.y += this.height;
					}
					break;
				case: 'x':
					enp.pos.x += mag;
					if (enp.pos.x>=this.width){
						enp.pos.x %= this.width;
					} else if (enp.pos.x < 0){
						enp.pos.x += this.width;
					}
					break;
				default:
					break;
			}
		}
	}
}
