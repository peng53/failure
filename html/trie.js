
class Trie {
	constructor(){
		this.root = new Node();
	}
	hasWord(word){
		const [p, r] = this.root.prefix(word);
		return r==word.length-1 && p.isWord==true;
	}
	addWord(word){
		this.root.addNodes(word).isWord = true;
	}
	get words(){
		let words = [];
		let q = new Queue();
		q.put(['', this.root]);
		let chars,node;
		while (q.len>0){
			[chars,node] = q.get();
			if (node.isWord==true){
				words.push(chars);
			}
			node.c.forEach(
				(p,i)=>{
					if (p) q.put([chars+chari(i), p]);
				}
			);
		}
		return words;
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
function chari(i){
	return String.fromCharCode(i+97);
}
