class Trie {
	constructor(){
		this.root = new Node();
	}
	/*
	addWord(word){
		let p = this.root;
		let i;
		for (const l of word){
			i = l.charCodeAt()-97;
			if (p.c[i]==null){
				p.c[i] = new Node();
			}
			p = p.c[i];
		}
		p.isWord = true;
	}*/
	hasWord(word){
		const [p, r] = this.root.prefix(word);
		return r==word.length-1 && p.isWord==true;
	}
	addWord(word){
		this.root.addNodes(word).isWord = true;
	}

}
class Node {
	constructor(){
		this.c = new Array(26);
		for (let i=0;i<26;++i){
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
			i = charc(pfx[r]);
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
			i = charc(pfx[r]);
			if (p){
				p.c[i] = new Node();
				p = p.c[i];
			}
		}
		return p;
	}
}

function charc(c){
	return c.toLowerCase().charCodeAt()-97;
}
