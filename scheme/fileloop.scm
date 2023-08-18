$script = '(let* ((image (car (gimp-file-load 1 "{0}" "{0}"))) (drawable (car (gimp-image-get-active-layer image)))	)	(gimp-image-convert-rgb image)	(gimp-image-convert-indexed image 0 3 2 0 1 "")(file-tiff-save 1 image drawable "{1}" "filename" 6))'




'
(let* 
	(
		(image (car (gimp-file-load 1 "{0}" "{0}")))
		(drawable (car (gimp-image-get-active-layer image)))
	)
	(gimp-image-convert-rgb image)
	(gimp-image-convert-indexed image 0 3 2 0 1 "")

	(file-tiff-save 1 image drawable "{1}" "filename" 6)
)
'


C:\Programs\GIMPPortable\App\gimp\bin\gimp-2.10.exe -i -b '(let* (
		(image (car (gimp-file-load 1 "r:/multi____lzw_0.tif" "r:/regtiff.png")))
		(drawable (car (gimp-image-get-active-layer image)))
	)
	(gimp-image-convert-rgb image)
	(gimp-image-convert-indexed image 0 3 2 0 1 "")

	(file-tiff-save 1 image drawable "r:/test.tiff" "filename" 6)
	
)'
__________________________________________________________________
(let* 
	(
		(filelist '(myfile1 myfile2 myfile3))
		(last 'NONE)
	)
	(while (not (null? filelist))
		(let*
			(
				(filename (car filelist))
			)
			(set!
				last filename
			)
		)
		(set! filelist (cdr filelist))
	)
	last
)
__________________________________________________________________
(
let* 
	(
		(filelist '(R:\multi____lzw_0.tif R:\multi____lzw_1.tif R:\multi____lzw_2.tif))
		(outlist '(R:\out\multi____lzw_0.tif R:\out\multi____lzw_1.tif R:\out\multi____lzw_2.tif))
	)
	(while (not (null? filelist))
		(let*
			(
				(filename (car filelist))
				(outname (car outlist))
				(image (car (gimp-file-load 1 filename filename)))
				(drawable (car (gimp-image-get-active-layer image)))
			)
			(gimp-image-convert-rgb image)
			(gimp-image-convert-indexed image 0 3 2 0 1 "")
			(file-tiff-save 1 image drawable "R:/out/multi____lzw_0.tif" "" 6)
			(gimp-image-delete image)
		)
		(set! filelist (cdr filelist))
		(set! outlist (cdr outlist))
	)
	filelist
)