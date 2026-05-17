function customNodes(charIt){
	const Ob = class {
		constructor(){
			this.c = new Array();
		}
		hasPrefix(pfx){
			let [p, r] = this.prefix(pfx);
			return r==pfx.length-1;
		}
		prefix(pfx){
			let p = this, i;
			for (let r=0,R=pfx.length;r<R;++r){
				if (!charIt.hasChar(pfx[r]) || !p.c[pfx[r]]){
					return [p, r-1];
				}
				p = p.c[pfx[r]];
			}
			return [p, pfx.length-1];
		}
		addNodes(pfx){
			let [p, r] = this.prefix(pfx);
			let i;
			++r;
			for (const R=pfx.length;r<R;++r){
				if (p && charIt.hasChar(pfx[r])){
					p.c[pfx[r]] = new Ob;
					p = p.c[pfx[r]];
				}
			}
			return p;
		}
	};
	return Ob;
}

class Trie {
	constructor(charit){
		this.charIt = charit
		this.NODE = customNodes(this.charIt);
		this.root = new this.NODE();
	}
	hasWord(word){
		const [p, r] = this.root.prefix(word);
		return r==word.length-1 && p.isWord==true;
	}
	addWord(word){
		this.root.addNodes(word).isWord = true;
	}
	get words(){
		return this.affixesNode(this.root);
	}
	affixesNode(ofNode){
		return Array.from(this.affixesG(ofNode));
	}
	affixes(pfx){
		const [p, r] = this.root.prefix(pfx);
		if (p && r==pfx.length-1){
			return this.affixesNode(p);
		} else {
			return [];
		}
	}
	*affixesG(ofNode){
		const q = new Queue();
		q.put(['', ofNode]);
		let chars,node;
		while (q.len>0){
			[chars,node] = q.get();
			if (node.isWord==true){
				yield(chars);
			}
			for (const c of this.charIt.chars()){
				if (c in node.c){
					q.put([chars+c, node.c[c]]);
				}
			}
			/*
			node.c.forEach(
				(p,i)=>{
					if (p) q.put([chars+this.hasher.tochar(i), p]);
				}
			);
			*/
		}
	}
}

const LowerAlphaHash = {
	toindex : (c) => c.toLowerCase().charCodeAt()-97,
	tochar : (i) => String.fromCharCode(i+97),
	len: 26
};
const PrintableHash = {
	toindex : (c) => c.charCodeAt()-32,
	tochar : (i) => String.fromCharCode(i+32),
	len: 95
};

class LowerAlphaCharIt {
	static *chars(){
		for (let i=97;i<123;++i){
			yield(String.fromCharCode(i));
		}
	}
	static hasChar(c){
		return (c>='a' && c<='z');
	}
}
