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
	let div = document.getElementById(ele);
	div.style.display = div.style.display == "none" ? "block" : "none";
}

function retrieveArgs(graph){
	let nm = [getValueFrom('xviewL'), getValueFrom('xviewR')].map(Number);
	let xview = (nm.some(isNaN) || nm[0] >= nm[1])
		? guessXview(graph)
		: nm;
	nm = [getValueFrom('yviewL'), getValueFrom('yviewR')].map(Number);
	let yview = (nm.some(isNaN) || nm[0] >= nm[1])
		? guessYview(graph)
		: nm;
	nm = [getValueFrom('gridx'), getValueFrom('gridy')].map(Number);
	let gridxy = (nm.some(isNaN) || nm.some(v => v<0))
		? undefined
		: nm;
	nm = [getValueFrom('axisx'), getValueFrom('axisy')].map(Number);
	let axisxy = nm.some(isNaN)
		? undefined
		: nm;
	nm = [getValueFrom('axisxn'), getValueFrom('axisyn')].map(Number);
	let axisxyn = (nm.some(isNaN) || nm.some(v => v<=0))
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
	let [xview, yview, gridxy, axisxy, axisxyn] = retrieveArgs(graph);
	setViews(xview, yview);
	let xscale = calculateScale(xview, can.width);
	let yscale = calculateScale(yview, can.height);
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
	let numbers = getValueFrom('points');
	if (numbers.length === 0){
		return;
	}
	let color = getValueFrom('ptcolor');
	let name = getValueFrom('name');
	let dsi = graph.datasets.length;
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
	let numbers = str.split(',').map(Number);
	for (let i=0,l=numbers.length; i<l; i+=2){
		if (isNaN(numbers[i]) || isNaN(numbers[i+1])){
			console.log('Skipping values '+i+', '+i+1);
			continue;
		}
		arr.push(new Point(numbers[i], numbers[i+1]));
	}
}

function addDatasetToList(dataset){
	let option = document.createElement('option');
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
	let options = document.getElementById('pointsList');
	while (options.length>0){
		options.remove(0);
	}
}
function deleteDataset(graph){
	let plist = document.getElementById('pointsList');
	let index = plist.selectedIndex;
	console.log(index);
	plist.remove(index);
	graph.datasets.splice(index,1);
}
function editDataset(graph){
	let index = document.getElementById('pointsList').selectedIndex;
	if (index > -1 ){
		let ds = graph.datasets[index];
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
	let index = document.getElementById('pointsList').selectedIndex;
	if (index > -1){
		// duplicate code of addPoints..
		let values = getValueFrom('points');
		if (values.length === 0){
			return;
		}
		let color = getValueFrom('ptcolor');
		let name = getValueFrom('name');
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
		let option = document.getElementById('pointsList')[index];
		option.text = name;
		option.style.color = color;
		setValueTo('points', '');
	}
}

function standardize_color(str){
	let ctx = document.createElement('canvas').getContext('2d');
	ctx.fillStyle = str;
	return ctx.fillStyle;
}
function setCanvasSize(){
	let can = document.getElementById('mycan');
	let axis = document.getElementById('axisl');
	let newHeight = getValueFrom('canh');
	let newWidth = getValueFrom('canw');
	can.width = newWidth;
	can.height = newHeight;
	canvasBg(can, getValueFrom('bgcolor'));
	axis.width = newWidth;
	axis.height = newHeight;
	let ctx = axis.getContext('2d');
	ctx.clearRect(0, 0, axis.width, axis.height);
}
function maxCanvasSize(){
	let topMenu = document.getElementById('topmenu');
	setValueTo('canw', window.innerWidth);
	setValueTo('canh', window.innerHeight - topMenu.offsetHeight);
}
