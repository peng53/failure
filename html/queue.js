'use strict';

function QNode(val){
	this.v = val;
	this.next = undefined;
}

function Queue(){
	this.head = undefined;
	this.tail = undefined;
	this.len = 0;
	this.put = function(val){
		let n = new QNode(val);
		if (this.len == 0){
			this.head = n;
		} else if (this.len = 1){
			this.head.next = n;
			this.tail = n;
		} else {
			this.tail.next = n;
		}
		++this.len;
	};
	this.get = function(){
		let n;
		if (this.len == 0){
			return;
		} else if (this.len == 1){
			n = this.head;
			this.head = undefined;
		} else {
			n = this.head;
			this.head = this.head.next;
		}
		--this.len;
		return n.v;
	};
}

function NaiveQueue(){
	this.data = [];
	this.len = 0;
	this.put = function(val){
		++this.len;
		this.data.push(val);
	};
	this.get = function(){
		if (this.len>0){
			--this.len;
			let val = this.data.shift();
			return val;
		}
	};
}