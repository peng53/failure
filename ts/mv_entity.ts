export class Entity {
	enMod : Modifier
	leMod : Modifier
	rng : () => [string,number]
	constructor(rng : ()=> [string,number]){
		this.rng = rng;
		this.enMod = null;
		this.leMod = null;
	}
}

export class Modifier {
	what : string
	change: number
	constructor(what: string, change: number){
		this.what = what;
		this.change = change;
	}
	mod(color : {r: number, g: number, b: number}){
		if (this.what in color){
			color[this.what] += this.change;
		}
		return color;
	}
}