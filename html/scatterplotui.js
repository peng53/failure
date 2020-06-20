var currentMenu = undefined;

function showMenu(menu){
	if (currentMenu === menu){
		toggleHide(currentMenu);
		currentMenu = undefined;
	} else {
		if (!(currentMenu === undefined)){
			toggleHide(currentMenu);
		}
		currentMenu = menu;
		toggleHide(currentMenu);
	}
}
function toggleHide(ele){
	const div = document.getElementById(ele);
	div.style.display = div.style.display == "none" ? "block" : "none";
}

function retrieveArgs(graph){
	let nm = [getValueFrom('xviewL'), getValueFrom('xviewR')].map(Number);
	const xview = (nm.some(isNaN) || nm[0] >= nm[1])
		? guessXview(graph)
		: nm;
	nm = [getValueFrom('yviewL'), getValueFrom('yviewR')].map(Number);
	const yview = (nm.some(isNaN) || nm[0] >= nm[1])
		? guessYview(graph)
		: nm;
	nm = [getValueFrom('gridx'), getValueFrom('gridy')].map(Number);
	const gridxy = (nm.some(isNaN) || nm.some(v => v<0))
		? undefined
		: nm;
	nm = [getValueFrom('axisx'), getValueFrom('axisy')].map(Number);
	const axisxy = nm.some(isNaN)
		? undefined
		: nm;
	nm = [getValueFrom('axisxn'), getValueFrom('axisyn')].map(Number);
	const axisxyn = (nm.some(isNaN) || nm.some(v => v<=0))
		? undefined
		: nm;
	return [xview, yview, gridxy, axisxy, axisxyn];
}
function setViews(xview, yview){
	setValueTo('xviewL', xview[0]);
	setValueTo('xviewR', xview[1]);
	setValueTo('yviewL', yview[0]);
	setValueTo('yviewR', yview[1]);
}
function drawGraph(graph, can){
	const [xview, yview, gridxy, axisxy, axisxyn] = retrieveArgs(graph);
	setViews(xview, yview);
	const xscale = calculateScale(xview, can.width);
	const yscale = calculateScale(yview, can.height);
	canvasBg(can, getValueFrom('bgcolor'));
	if (gridxy){
		gridLines(xscale, xview, gridxy[0], yscale, yview, gridxy[1], can, hcolor=getValueFrom('grcolor'), vcolor=getValueFrom('grcolor'));
		//tickNumbers(xscale, xview, gridxy[0], yscale, yview, gridxy[1], can);
	}
	if (axisxy && axisxyn){
		//axisLines(xscale, xview, yscale, yview, can, color=getValueFrom('orcolorx'), axisxy, axisxyn);
		axisLines(xview, yview, axisxy, axisxyn, hcolor=getValueFrom('orcolorx'), vcolor=getValueFrom('orcolory'), fontsize=getValueFrom('alfontsize'));
	}
	for (let i=0, l=graph.datasets.length; i<l; ++i){
		plotPoints(graph.datasets[i].points, xscale, yscale, xview, yview, can, graph.datasets[i].color, noLine=(!graph.datasets[i].connectPoints));
	}
}

function getValueFrom(name){
	return (document.getElementById(name).value.length > 0)
		? document.getElementById(name).value
		: undefined;
}
function setValueTo(name, val){
	document.getElementById(name).value = val;
}
function addPoints(graph){
	const numbers = getValueFrom('points');
	if (numbers.length === 0){
		return;
	}
	const color = getValueFrom('ptcolor');
	const name = getValueFrom('name');
	const dsi = graph.datasets.length;
	if (name.length === 0){
		name = 'Dataset #'+dsi;
	}
	graph.datasets.push(new Dataset(name, color, getValueFrom('connectPoints')));
	parseStrToArray(numbers, graph.datasets[dsi].points);
	graph.datasets[dsi].points.sort((a,b) => a.x-b.x);
	addDatasetToList(graph.datasets[dsi]);
	setValueTo('points','');
}

function parseStrToArray(str, arr){
	// Parse string of comma separated x,y coordinates to array of Points
	const numbers = str.split(',').map(Number);
	for (let i=0,l=numbers.length; i<l; i+=2){
		if (isNaN(numbers[i]) || isNaN(numbers[i+1])){
			console.log('Skipping values '+i+', '+i+1);
			continue;
		}
		arr.push(new Point(numbers[i], numbers[i+1]));
	}
}

function addDatasetToList(dataset){
	const option = document.createElement('option');
	option.text = dataset.name;
	option.style.color = dataset.color;
	document.getElementById('pointsList').add(option);
}
function guessArgs(graph){
	setViews(guessXview(graph),guessYview(graph));
}
function guessXview(graph){
	return getMinAndMaxOverAll(graph.datasets, (point) => point.x);
}
function guessYview(graph){
	return getMinAndMaxOverAll(graph.datasets, (point) => point.y);
}
function clearDatasets(graph){
	graph.datasets = [];
	const options = document.getElementById('pointsList');
	while (options.length>0){
		options.remove(0);
	}
}
function deleteDataset(graph){
	const plist = document.getElementById('pointsList');
	const index = plist.selectedIndex;
	console.log(index);
	plist.remove(index);
	graph.datasets.splice(index,1);
}
function editDataset(graph){
	const index = document.getElementById('pointsList').selectedIndex;
	if (index > -1 ){
		const ds = graph.datasets[index];
		let out = '';
		for (let i=0, l=ds.points.length; i<l; ++i){
			if (i != 0){
				out += ',\n';
			}
			out += ds.points[i].x + ','+ds.points[i].y;
		}
		setValueTo('points', out);
		setValueTo('ptcolor', standardize_color(ds.color));
		setValueTo('name', ds.name);
		document.getElementById('connectPoints').checked = ds.connectPoints;
	}
}
function saveDataset(graph){
	const index = document.getElementById('pointsList').selectedIndex;
	if (index > -1){
		// duplicate code of addPoints..
		const values = getValueFrom('points');
		if (values.length === 0){
			return;
		}
		const color = getValueFrom('ptcolor');
		const name = getValueFrom('name');
		if (name.length === 0){
			name = 'Dataset #'+index;
		}
		graph.datasets[index].color = color;
		graph.datasets[index].name = name;
		graph.datasets[index].connectPoints = document.getElementById('connectPoints').checked;
		graph.datasets[index].points = [];

		parseStrToArray(values, graph.datasets[index].points);
		graph.datasets[index].points.sort((a,b) => a.x-b.x);
		// change the select now..
		const option = document.getElementById('pointsList')[index];
		option.text = name;
		option.style.color = color;
		setValueTo('points', '');
	}
}

function standardize_color(str){
	const ctx = document.createElement('canvas').getContext('2d');
	ctx.fillStyle = str;
	return ctx.fillStyle;
}
function setCanvasSize(){
	const can = document.getElementById('mycan');
	const axis = document.getElementById('axisl');
	const newHeight = getValueFrom('canh');
	const newWidth = getValueFrom('canw');
	can.width = newWidth;
	can.height = newHeight;
	canvasBg(can, getValueFrom('bgcolor'));
	axis.width = newWidth;
	axis.height = newHeight;
	const ctx = axis.getContext('2d');
	ctx.clearRect(0, 0, axis.width, axis.height);
}
function maxCanvasSize(){
	const topMenu = document.getElementById('topmenu');
	setValueTo('canw', window.innerWidth);
	setValueTo('canh', window.innerHeight - topMenu.offsetHeight);
}
