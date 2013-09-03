#!/usr/bin/env python3.3

import os
import sys
import yaml

def counting_tags(dir_stemmed_tags_folder, tags_hash):
	## check validity of input arguments
	if not dir_stemmed_tags_folder.endswith('/'):
		dir_stemmed_tags_folder += '/'

	if not isinstance(tags_hash, dict):
		print( '[Error] arg 2 is not a dict type' )
		raise AttributeError

	if not os.path.isdir(dir_stemmed_tags_folder):
		print( '[Error] Folder for stemmed tags', dir_stemmed_tags_folder, 'does not exist.')
		raise IOError

	# file walker
	for tagfile_name in os.listdir(dir_stemmed_tags_folder):
		path = dir_stemmed_tags_folder + tagfile_name

		insert_to_dict( path, tags_hash )

def insert_to_dict(tag_file_path, tags_hash):
	if not os.path.isfile(tag_file_path):
		print( '[Error] Tag file', tag_file_path, 'does not exist.')
		raise IOError

	fhndl = open(tag_file_path, 'r')

	for tag in fhndl:
		tag = tag.strip()
		if tag == '': continue

		if tag not in tags_hash:
			tags_hash[tag] = 1
		else:
			tags_hash[tag] += 1

	fhndl.close()

if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) != 2:
		print( 'Usage: ./unique_tags.py [setting_file]\n' )
		os._exit(0)

	## load a setting file (yaml)
	yml = yaml.load(open(sys.argv[1]))

	## ask to run feature extraction
	#want_to_run = input('Do you want to proceed tag stemming from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
	#if want_to_run != 'yes':
	#	os._exit(0)

	## hash for tags (key is a tag, value is a frequency)
	tags_hash = dict()

	## feature extraction
	for key in yml['datasets'].keys():
		meta = yml['datasets'][key]

		dir_stemmed_tags_folder = yml['path']['base'] + meta['stemmed_tags_folder']

		if not os.path.exists(dir_stemmed_tags_folder):
			print('[Error] Folder for stemmed tags', dir_stemmed_tags_folder, 'does not exist')
			raise IOError

		print( '\nProcessing', dir_stemmed_tags_folder, 'folder.')

		counting_tags( dir_stemmed_tags_folder, tags_hash )

		print ( len (tags_hash) )
