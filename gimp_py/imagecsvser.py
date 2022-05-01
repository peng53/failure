import os
from gimp import pdb

def saveFlatDpiPngFn(outdir, filenamefmt, dpi):
	"""
	Returns closure that saves image to outdir with filenamefmt
	filenamefmt should have {0} in it to denote layer number.
	Use this as example for other save types, operations
	"""
	def fn(image, i):
		name = filenamefmt.format(i)
		image.flatten()
		pdb.gimp_image_set_resolution(image, dpi, dpi)
		pdb.file_png_save(image,image.active_drawable,os.path.join(outdir,name),name,0,0,0,0,0,0,0)
	return fn

def setLayerText(textLayer, text):
	"""
	Set text on text layer to text. :-)
	"""
	pdb.gimp_text_layer_set_text(textLayer,text)

def render(image, copies, layerMap, savefn):
	"""
	copies is list of dictionaries with text
	layerMap is dictionary of which layer the text will go
	savefn is function to save resulting image with
	"""
	dlayers = {l.name:l for l in image.layers}
	toEdits = {k:image.layers[layerMap[k]] if layerMap[k]!=None else dlayers[k] for k in layerMap}
	for i,copy in enumerate(copies):
		for key in toEdits:
			setLayerText(toEdits[key], copy[key])
		result = pdb.gimp_image_duplicate(image)
		savefn(result, i)
		pdb.gimp_image_delete(result)

def example():
	"""
	load image of which 0th and 2nd layer are text layers (examine image.layers for exact order)
	create savefn that saves to r:/ with name 'seq-{0}-image.png and 200 dpi
	(not sure if 200 dpi is working)
	create list of pictures to render, an image for each item of which contains data
	create dictionary of layer to key mapping
	calls render function building the images
	and frees image
	"""
	image = pdb.gimp_file_load('r:/textlayers.xcf','textlayers.xcf')
	savefn = saveFlatDpiPngFn('r:/', 'seq-{0}-image.png', 200)
	copies = [{'first':'first card','second':'first tagline'}
	, {'first':'second card','second':'second tagline'}]
	layMap = {'first':0, 'second':2}
	render(image, copies, layMap, savefn)
	pdb.gimp_image_delete(image)

"""
import sys
sys.path.append("PATH CONTAINING PYTHON SCRIPTS")
import imagecsvser
imagecsvser.example()
"""