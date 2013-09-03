#!/usr/bin/env python3.3

import os
import pickle
import sys
import tempfile
import yaml

MAX_CHUNK_SIZE = 10000

def save_temporally(local_tags_hash):
	## check validity of input arguments
	if not isinstance(local_tags_hash, dict):
		print( '[Error] arg 2 is not a dict type' )
		raise AttributeError

	with tempfile.NamedTemporaryFile(mode='wb', delete=False) as f:
		pickle.dump(local_tags_hash, f)

	return f.name

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
	local_tags_hash = dict()
	cnt = 0
	tmp_filenames = []

	for tagfile_name in os.listdir(dir_stemmed_tags_folder):
		path = dir_stemmed_tags_folder + tagfile_name

		if cnt == MAX_CHUNK_SIZE:
			tmp_filenames.append( save_temporally(local_tags_hash) )
			cnt = 0
			local_tags_hash.clear()

		insert_to_dict( path, local_tags_hash )
		cnt += 1

	# save remaining local_tags_hash
	tmp_filenames.append( save_temporally(local_tags_hash) )

	# merge temporary local_tags_hashes
	for tmp_file in tmp_filenames:
		with open(tmp_file, 'rb') as f:
			tmp_tags_hash = pickle.load(f)
			tags_hash.update(tmp_tags_hash)

		os.remove(tmp_file)

def insert_to_dict(tag_file_path, tags_hash):
	if not os.path.isfile(tag_file_path):
		print( '[Error] Tag file', tag_file_path, 'does not exist.')
		raise IOError

	with open(tag_file_path, 'r') as f:
		for tag in f:
			tag = tag.strip()
			if tag == '': continue

			if tag not in tags_hash:
				tags_hash[tag] = 1
			else:
				tags_hash[tag] += 1

if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) < 2:
		print ('Usage: ./unique_tags.py [dataset_setting_file] [output_file]\n')
		os._exit(0)

	## load a setting file (yaml)
	with open(sys.argv[1]) as f:
		yml = yaml.load(f)

	## recommend an output filename (when user does not enter the output filename)
	if len(sys.argv) < 3:
		recommend = input("You didn't enter the output filename. Do you want to \
use " + yml['dataset_name'] +"_unique_tags.txt as an output filename? [yes/no]: ")
		if recommend != 'yes': os._exit(0)
		else: output_filename = yml['dataset_name'] + '_unique_tags.txt'
	else:
		output_filename = sys.argv[2]

	## ask to run feature extraction
	want_to_run = input('Do you want to count unique tags from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
	if want_to_run != 'yes': os._exit(0)

	## hash for tags (key is a tag, value is a frequency)
	tags_hash = dict()

	## feature extraction
	for key in yml['datasets'].keys():
		meta = yml['datasets'][key]

		dir_stemmed_tags_folder = yml['path']['base'] + meta['stemmed_tags_folder']

		if not os.path.exists(dir_stemmed_tags_folder):
			print('[Error] Folder for stemmed tags', dir_stemmed_tags_folder, 'does not exist')
			raise IOError

		print ('\nProcessing', dir_stemmed_tags_folder, 'folder.')

		counting_tags( dir_stemmed_tags_folder, tags_hash )

	## save results into output_file
	print ('Saving results...')
	with open(output_filename, 'w') as f:
		for key, value in tags_hash.items():
			f.write(key + '\t' + str(value) + '\n')
