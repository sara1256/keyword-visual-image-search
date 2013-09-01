#!/bin/sh

BASEDIR=$(dirname $0)

compute_descriptors="$BASEDIR/../../../3rd_party/inria_softwares/descriptor_extractors/compute_descriptors_linux64"

if [ $# -eq 2 ]
then
	# split path and filename from filepath
	IMG_PATH=${1%/*}
	IMG_NAME=${1##*/}

	echo "- rescaling and intensity normalization..."
	#djpeg $1 | ppmtopgm | pnmnorm -bpercent=0.01 -wpercent=0.01 | pnmscale -pixels 786432 > $BASEDIR/tmp/$IMG_NAME.pnm
	djpeg $1 | ppmtopgm | pnmnorm -bpercent=0.01 -wpercent=0.01 | pnmscale -pixels 786432 > /tmp/$IMG_NAME.pnm

	echo "- computing descriptors..."
	#$compute_descriptors -i $BASEDIR/tmp/$IMG_NAME.pnm -o4 $2/$IMG_NAME.siftgeo -hesaff -sift
	$compute_descriptors -i /tmp/$IMG_NAME.pnm -o4 $2/$IMG_NAME.siftgeo -hesaff -sift

	echo "- removing temporary pnm image..."
	#rm $BASEDIR/tmp/$IMG_NAME.pnm
	rm /tmp/$IMG_NAME.pnm

	exit
else
	echo "Usage: ./compute_siftgeo [image_path] [output_siftgeo_directory]"
	exit
fi
