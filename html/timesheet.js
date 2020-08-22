'use strict';
function formatNotes(data){
	const lines = data.split('\n');
	const outv = [];
	const codes = {'PROD' : 0};
	let ts, te, code, out, diff;
	for (let i=0,m=lines.length;i<m;i+=4){
		// lines[i+3] is ignored as an empty line
		if (lines[i].length === 0 || lines[i+1].length === 0 || lines[i+2].length === 0){
			console.log('Invalid at line = '+i);
			break;
		}
		ts = new Date(Date.parse(lines[i]));
		if (te){
			diff = (ts - te)/60000;
			if (diff > 0){
				codes['PROD'] += diff;
				outv.push({code: 'ATO', start: niceTime(te,':'), end: niceTime(ts,':'), total: (diff/60).toFixed(2)});
			}
		}
		code = lines[i+1];
		te = new Date(Date.parse(lines[i+2]));
		diff = (te - ts)/60000;
		if (diff > 0){
			if (!(code in codes)){
				codes[code] = 0;
			}
			codes[code] += diff;
			outv.push({code, start: niceTime(ts,':'), end: niceTime(te,':'), total: (diff/60).toFixed(2)});
		}
	}
	return outv;
}
function niceTime(td, sep){
	let out = ''
	if (td.getHours()<10){
		out += ' ';
	}
	out += td.getHours();
	out += sep;
	if (td.getMinutes()<10){
		out += '0';
	}
	out += td.getMinutes();
	return out;
}
