function customNodes(hasher){
	const Ob = class {
		constructor(){
			this.c = new Array(hasher.len);
			for (let i=0;i<hasher.len;++i){
				this.c[i] = null;
			}
		}
		hasPrefix(pfx){
			let [p, r] = this.prefix(pfx);
			return r==pfx.length-1;
		}
		prefix(pfx){
			let p = this, i;
			for (let r=0,R=pfx.length;r<R;++r){
				i = hasher.toindex(pfx[r]);
				if (!p.c[i]){
					return [p, r-1];
				}
				p = p.c[i];
			}
			return [p, pfx.length-1];
		}
		addNodes(pfx){
			let [p, r] = this.prefix(pfx);
			let i;
			++r;
			for (const R=pfx.length;r<R;++r){
				i = hasher.toindex(pfx[r]);
				if (p){
					p.c[i] = new Ob;
					p = p.c[i];
				}
			}
			return p;
		}
	};
	return Ob;
}

class Trie {
	constructor(hasher, hlen){
		this.hasher = hasher;
		this.NODE = customNodes(this.hasher);
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
		const afx = [];
		const q = new Queue();
		q.put(['', ofNode ? ofNode : this.root]);
		let chars,node;
		while (q.len>0){
			[chars,node] = q.get();
			if (node.isWord==true){
				afx.push(chars);
			}
			node.c.forEach(
				(p,i)=>{
					if (p) q.put([chars+this.hasher.tochar(i), p]);
				}
			);
		}
		return afx;
	}
	affixes(pfx){
		const [p, r] = this.root.prefix(pfx);
		if (p && r==pfx.length-1){
			return this.affixesNode(p);
		} else {
			return [];
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
