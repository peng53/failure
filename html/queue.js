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
		if (this.tail == undefined){
			this.tail = new QNode(val);
			this.head = this.tail;
		} else {
			this.tail.next = new QNode(val);
			this.tail = this.tail.next;
		}
		++this.len;
	};
	this.push = this.put;
	this.get = function(){
		if (this.head == undefined) return undefined;
		if (this.head == this.tail){
			this.tail = undefined;
		}
		let ret = this.head.v;
		this.head = this.head.next;
		--this.len;
		return ret;
	};
}

function NaiveQueue(){
	this.data = [];
	this.len = 0;
	this.put = function(val){
		++this.len;
		this.data.push(val);
	};
	this.push = this.put;
	this.get = function(){
		if (this.len>0){
			--this.len;
			let val = this.data.shift();
			return val;
		}
	};
}