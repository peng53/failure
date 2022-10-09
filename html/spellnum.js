// https://support.microsoft.com/en-us/office/convert-numbers-into-words-a0d166fb-e1ea-4090-95c8-69442cd55d98
// https://github.com/peng53/powers/blob/main/num2words.ps1

function Num2Words(str_n){
	let [num,cents] = str_n.split('.',2).map(e=>parseInt(e));
	cents = (!cents || isNaN(cents) || cents<1) ? 'No Cents' : `${cents}/100`;
	if (!num || isNaN(num) || num<1){
		return `No Dollars and ${cents}`;
	}
	num = num.toString();
	const tens = function(t){
		const tensScale = ['', '', ' Twenty', ' Thirty', ' Forty', ' Fifty', ' Sixty', ' Seventy', ' Eighty', ' Ninety'];
		const smallNum = ['', ' One', ' Two', ' Three', ' Four', ' Five', ' Six', ' Seven', ' Eight', ' Nine', ' Ten', ' Eleven', ' Twelve', ' Thirteen', ' Fourteen', ' Fifteen', ' Sixteen', ' Seventeen', ' Eighteen', ' Nineteen'];
		if (t<20){
			return smallNum[t];
		}
		return tensScale[Math.floor(t/10)] + smallNum[t%10];
	};
	const hundreds = function(h){
		if (parseInt(h) == 0){
			return '';
		}
		h = h.padStart(3,'0');
		let r = '';
		if (h[0] != '0'){
			r = tens(Math.floor(parseInt(h)/100)) + ' Hundred';
		}
		return r + tens(parseInt(h.substr(1,2)));
	};
	let count = 1;
	let dollar = '';
	let l,temp;
	const places = ['', '', ' Thousand', ' Million', ' Billion', ' Trillion'];
	for (l=num.length; l>2; l-=3){
		temp = hundreds(num.substr(l-3,3));
		if (temp.length > 0){
			dollar = temp + places[count]+dollar;
		}
		count++;
	}
	if (l > 0){
		temp = hundreds(num.substr(0,l));
		if (temp.length > 0){
			dollar = temp + places[count]+dollar;
		}
	}
	return (dollar + ' and ' + cents).trim();
}