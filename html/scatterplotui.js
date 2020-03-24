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

function retrieveArgs(){
	let xview, yview, gridxy;
	let nm = [getValueFrom('xviewL'), getValueFrom('xviewR')];
	if (nm[0] && nm[1]){
		nm = nm.map(Number);
		if (nm[0] < nm[1]){
			xview = nm;
		}
	}
	nm = [getValueFrom('yviewL'), getValueFrom('yviewR')];
	if (nm[0] && nm[1]){
		nm = nm.map(Number);
		if (nm[0] < nm[1]){
			yview = nm;
		}
	}
	nm = [getValueFrom('gridx'), getValueFrom('gridy')];
	if (nm[0] && nm[1]){
		nm = nm.map(Number);
		if (nm[0]>0 && nm[1]>0){
			gridxy = nm;
		}
	}
	return [xview, yview, gridxy];
}
function drawGraph(graph, can){
	let [xview, yview, gridxy] = retrieveArgs();

	if (xview === undefined){
		xview = getMinAndMaxOverAll(graph.datasets, (point) => point.x);
		setValueTo('xviewL', xview[0]);
		setValueTo('xviewR', xview[1]);
	}
	if (yview === undefined){
		yview = getMinAndMaxOverAll(graph.datasets, (point) => point.y);
		setValueTo('yviewL', yview[0]);
		setValueTo('yviewR', yview[1]);
	}
	let xscale = calculateScale(xview, can.width);
	let yscale = calculateScale(yview, can.height);
	canvasBg(can, getValueFrom('bgcolor'));
	if (gridxy){
		gridLines(xscale, xview, gridxy[0], yscale, yview, gridxy[1], can, hcolor=getValueFrom('grcolor'), vcolor=getValueFrom('grcolor'));
	}
	originLines(xscale, xview, yscale, yview, can, color=getValueFrom('orcolor'));
	for (let i=0, l=graph.datasets.length; i<l; ++i){
		plotPoints(graph.datasets[i].points, xscale, yscale, xview, yview, can, graph.datasets[i].color);
	}
}

function getValueFrom(name){
	return document.getElementById(name).value;
}
function setValueTo(name, val){
	document.getElementById(name).value = val;
}
function addPoints(graph){
	let values = getValueFrom('points').split(',').map(Number);
	let color = getValueFrom('ptcolor');
	let name = getValueFrom('name');
	let dsi = graph.datasets.length;
	if (name.length === 0){
		name = 'Dataset #'+dsi;
	}
	if (color.length === 0){
		color = 'black';
	}
	graph.datasets.push(new Dataset(name, color));
	for (let i=0, l=values.length; i<l; i+=2){
		graph.datasets[dsi].points.push(new Point(values[i], values[i+1]));
	}
	graph.datasets[dsi].points.sort((a,b) => a.x-b.x);
	addDatasetToList(graph.datasets[dsi]);
	document.getElementById('points').value = '';
}
function addDatasetToList(dataset){
	let option = document.createElement('option');
	option.text = dataset.name;
	option.style.color = dataset.color;
	document.getElementById('pointsList').add(option);
}
function guessArgs(graph){
	let xview = getMinAndMaxOverAll(graph.datasets, (point) => point.x);
	setValueTo('xviewL', xview[0]);
	setValueTo('xviewR', xview[1]);
	let yview = getMinAndMaxOverAll(graph.datasets, (point) => point.y);
	setValueTo('yviewL', yview[0]);
	setValueTo('yviewR', yview[1]);
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
	plist.remove(index);
	graph.datasets.pop(index);
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
		document.getElementById('points').value = out;
		document.getElementById('ptcolor').value = standardize_color(ds.color);
		document.getElementById('name').value = ds.name;
	}
}
function saveDataset(graph){
	let index = document.getElementById('pointsList').selectedIndex;
	if (index > -1){
		// duplicate code of addPoints..
		let values = getValueFrom('points').split(',').map(Number);
		let color = getValueFrom('ptcolor');
		let name = getValueFrom('name');
		if (name.length === 0){
			name = 'Dataset #'+index;
		}
		graph.datasets[index].color = color;
		graph.datasets[index].name = name;
		graph.datasets[index].points = [];
		for (let i=0, l=values.length; i<l; i+=2){
			graph.datasets[index].points.push(new Point(values[i], values[i+1]));
		}
		graph.datasets[index].points.sort((a,b) => a.x-b.x);
		// change the select now..
		let option = document.getElementById('pointsList')[index];
		option.text = name;
		option.style.color = color;
		document.getElementById('points').value = '';
	}
}

function standardize_color(str){
	let ctx = document.createElement('canvas').getContext('2d');
	ctx.fillStyle = str;
	return ctx.fillStyle;
}
function setCanvasSize(){
	let width = getValueFrom('canw');
	let height = getValueFrom('canh');
	can.width = width;
	can.height = height;
	canvasBg(can, getValueFrom('bgcolor'));
}
