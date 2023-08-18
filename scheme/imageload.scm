(let* (
		(image (car (gimp-file-load 1 "r:/multi____lzw_0.tif" "r:/regtiff.png")))
		(drawable (car (gimp-image-get-active-layer image)))
	)
	(gimp-image-convert-rgb image)
	(gimp-image-convert-indexed image 0 3 2 0 1 "")

	(file-tiff-save 1 image drawable "r:/test.tiff" "filename" 6)
	
)