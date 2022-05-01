import os
from gimp import pdb

def savePngFn(outdir, filenamefmt):
	"""
	Returns closure that saves image to outdir with filenamefmt
	filenamefmt should have {0} in it to denote layer number.
	Use this as example for other save types, operations
	"""
	def fn(image, i):
		name = filenamefmt.format(i)
		pdb.file_png_save(image,image.active_drawable,os.path.join(outdir,name),name,0,0,0,0,0,0,0)
	return fn

def layerSave(image, layer, savefn, i):
	"""
	Save layer of image with savefn as number i
	"""
	pdb.gimp_image_set_active_layer(image, layer)
	if pdb.gimp_edit_copy(image.active_drawable):
		dup = pdb.gimp_edit_paste_as_new_image()
		savefn(dup,i)
		pdb.gimp_image_delete(dup)
		return True
	else:
		return False

def layers2sepImg(image, savefn):
	"""
	Saves individual layers of an image to single layer images
	savefn is a function that saves the images.
	"""
	for i,lay in enumerate(reversed(image.layers)):
		if not layerSave(image,lay,savefn,i):
			print 'Save failed %d' %(i)

def example():
	"""
	Example:
		load image 3layer2.xcf from r:/
		create a save function that saves to r:/ with 
		format 'layer-{0}-image.png' as a png.
		separate with titular function
		delete/free image
	"""
	image = pdb.gimp_file_load('r:/3layer2.xcf','3layer2.xcf')
	savefn = savePngFn('r:/', 'layer-{0}-image.png')
	layers2sepImg(image, savefn)
	pdb.gimp_image_delete(image)

"""
import sys
sys.path.append("PATH CONTAINING PYTHON SCRIPTS")
import layersToSeparateImages
layersToSeparateImages.example()
"""