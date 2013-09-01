#!/usr/bin/env python3.3

import os
import sys
import yaml

CMD_EXTRACTOR_DISTRACTORS = './compute_siftgeo.sh %s %s'

def compute_siftgeo(image_folder_path, feature_folder_path):
	## check validity of input arguments
	if not image_folder_path.endswith('/'):
		image_folder_path = image_folder_path + '/'

	if not feature_folder_path.endswith('/'):
		feature_folder_path = feature_folder_path + '/'

	if not os.path.isdir(image_folder_path):
		print('[Error] Image folder', image_folder_path, 'does not exist.')
		raise IOError

	if not os.path.isdir(feature_folder_path):
		print('[Error] Feature folder', feature_folder_path, 'does not exist.')
		raise IOError

	## file walker
	for image_name in os.listdir(image_folder_path):
		image_path = image_folder_path + image_name

		if os.path.isfile(image_path):
			# split filename-string into filename and its extension (i.e. .jpg, .png)
			image_filename, image_file_extension = os.path.splitext(image_name)

			os.system( CMD_EXTRACTOR_DISTRACTORS % (image_path, feature_folder_path) )

if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) != 2:
		print( 'Usage: ./compute_all.py [setting_file]\n' )
		os._exit(0)

	## load a setting file (yaml)
	yml = yaml.load(open(sys.argv[1]))

	## ask to run feature extraction
	want_to_run = input('Do you want to proceed feature extraction from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
	if want_to_run != 'yes':
		os._exit(0)

	## feature extraction
	for key in yml['datasets'].keys():
		meta = yml['datasets'][key]
		dataset_image_folder = yml['path']['base'] + meta['images_folder']
		dataset_feature_folder = yml['path']['base'] + meta['siftgeos_folder']

		if not os.path.exists(dataset_image_folder):
			print('[Error] Image folder', dataset_image_folder, 'does not exist.')
			raise IOError

		if not os.path.exists(dataset_feature_folder):
			print('[Error] Feature folder', dataset_feature_folder, 'does not exist')
			raise IOError

		print( '\nProcessing', dataset_image_folder, 'folder.')

		compute_siftgeo(dataset_image_folder, dataset_feature_folder)
