image = pdb.gimp_image_new(1000, 600, 0)
bglay = pdb.gimp_layer_new(image, 1000, 600, 0, 'Background', 100, 0)
pdb.gimp_context_set_foreground('white')
bglay.fill(0)
image.insert_layer(bglay)
pdb.gimp_context_set_foreground('black')
txtlay = pdb.gimp_text_layer_new(image, 'first', 'Arial', 1, 1)
image.insert_layer(txtlay)
# get offset with txtlay.offsets
txtlay.translate(100,10)
pdb.gimp_text_layer_set_text(txtlay,'hi423423')
display = pdb.gimp_display_new(image)

copy_to_flat = pdb.gimp_image_duplicate(image)
pdb.gimp_display_new(copy_to_flat)
copy_to_flat.flatten()
pdb.gimp_image_set_resolution(copy_to_flat, 200, 200)


merged = pdb.gimp_image_merge_visible_layers(image, 0)

image = pdb.gimp_file_load('r:/3layer2.xcf', '3layer2.xcf')


for i,lay in enumerate(reversed(image.layers)):
	pdb.gimp_image_set_active_layer(image, lay)
	if pdb.gimp_edit_copy(image.active_drawable):
		dup = pdb.gimp_edit_paste_as_new_image()
		fn = '{0}_dups.png'.format(i)
		pdb.file_png_save(dup,dup.active_drawable,'r:/'+fn,fn,0,0,0,0,0,0,0)
		pdb.gimp_image_delete(dup)
	else:
		print 'fail %d' %(i)


for i,x in enumerate(image.layers):
	fn = '{0}_dups.png'.format(i)
	pdb.gimp_edit_copy(image.active_drawable)
	dup = pdb.gimp_edit_paste_as_new_image()
	pdb.file_png_save(dup,dup.active_drawable,'r:/'+fn,fn,0,0,0,0,0,0,0)
	pdb.gimp_image_delete(dup)




dup = pdb.gimp_image_new(image.width, image.height, 0)
lay = image.layers[0].copy()
dup.insert_layer(lay)


dup2 = pdb.gimp_edit_paste_as_new_image()
display = pdb.gimp_display_new(dup2)

image = pdb.gimp_file_load('r:/textlayers.xcf', 'textlayers.xcf')
display = pdb.gimp_display_new(image)

pdb.gimp_text_layer_set_text(image.layers[0],'hi423423')

