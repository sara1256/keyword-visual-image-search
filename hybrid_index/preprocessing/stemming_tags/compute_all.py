#!/usr/bin/env python3.3

import os
import sys
import yaml

CMD_STEMMER = './stemmer %s %s'

def stemming(dir_tags_folder, dir_stemmed_tags_folder):
	## check validity of input arguments
	if not dir_tags_folder.endswith('/'):
		dir_tags_folder = dir_tags_folder + '/'

	if not dir_stemmed_tags_folder.endswith('/'):
		dir_stemmed_tags_folder = dir_stemmed_tags_folder + '/'

	if not os.path.isdir(dir_tags_folder):
		print('[Error] Tag folder', dir_tags_folder, 'does not exist.')
		raise IOError

	if not os.path.isdir(dir_stemmed_tags_folder):
		print('[Error] Folder for stemmed tags', dir_stemmed_tags_folder, 'does not exist.')
		raise IOError

	## file walker
	for tagfile_name in os.listdir(dir_tags_folder):
		tagfile_path = dir_tags_folder + tagfile_name
		stemmed_tagfile_path = dir_stemmed_tags_folder + 'stemmed_' + tagfile_name

		if os.path.isfile(tagfile_path):
			os.system( CMD_STEMMER % (tagfile_path, stemmed_tagfile_path) )


if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) != 2:
		print( 'Usage: ./compute_all.py [setting_file]\n' )
		os._exit(0)

	## load a setting file (yaml)
	yml = yaml.load(open(sys.argv[1]))

	## ask to run stemming
	want_to_run = input('Do you want to proceed tag stemming from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
	if want_to_run != 'yes':
		os._exit(0)

	## stemming
	for key in yml['datasets'].keys():
		meta = yml['datasets'][key]

		dir_tags_folder = yml['path']['base'] + meta['tags_folder']
		dir_stemmed_tags_folder = yml['path']['base'] + meta['stemmed_tags_folder']

		if not os.path.exists(dir_tags_folder):
			print('[Error] Tag folder', dir_tags_folder, 'does not exist.')
			raise IOError

		if not os.path.exists(dir_stemmed_tags_folder):
			os.mkdir(dir_stemmed_tags_folder)
			#print('[Error] Folder for stemmed tags', dir_stemmed_tags_folder, 'does not exist')
			#raise IOError

		print( '\nProcessing', dir_tags_folder, 'folder.')

		stemming( dir_tags_folder, dir_stemmed_tags_folder )
