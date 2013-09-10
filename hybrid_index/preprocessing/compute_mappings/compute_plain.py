#!/usr/bin/env python3.3

import os
import sys
import yaml

def sort_images_by_name(image_folder_path):
	filenames = filter(lambda x: x, os.listdir(image_folder_path))
	ids = map(lambda x: int(os.path.splitext(x)[0]), filenames)
	ids = sorted(ids)

	return map(lambda x: str(x) + '.jpg', ids)

unique_id = 0

def compute_mappings(ymln_basepath, ymln_dataset, fout):
	global unique_id

	images_folder = ymln_dataset['images_folder']
	vlads_folder = ymln_dataset['vlads_folder']
	tags_folder = ymln_dataset['tags_stemmed_folder']

	## check validity of input arguments
	images_dir_path = ymln_basepath + images_folder
	if not os.path.isdir(images_dir_path):
		print('[Error] Image folder', images_folder, 'does not exists.')
		raise IOError

	if not images_dir_path.endswith('/'): images_dir_path += '/'

	print ('Processing', images_folder, 'folder.')

	## file walker
	sorted_image_names = sort_images_by_name(images_dir_path)

	for image_name in sorted_image_names:
		vlad_name = vlads_folder + image_name + '.vlad'
		tags_name = tags_folder + 'stemmed_tags' + os.path.splitext(image_name)[0] + '.txt'

		fout.write('%s\t%s\t%s\t%s\n' % (unique_id, images_folder + image_name, vlad_name, tags_name))

		unique_id += 1

if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) != 2:
		print( 'Usage: ./compute_plain.py [setting_file]\n' )
		os._exit(0)

	## load a setting file (yaml)
	yml = yaml.load(open(sys.argv[1]))

	## ask to run image to unique_id mapping
	want_to_run = input('Do you want to proceed mapping dataset item to unique id (uid) from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
	if want_to_run != 'yes':
		os._exit(0)

	## widely used yml nodes
	ymln_basepath = yml['path']['base']
	ymln_integrated = yml['path']['integrated']
	ymln_integrated_folder = ymln_integrated['base_folder']
	ymln_integrated_mappings = ymln_integrated['mappings']

	integrated_folder = ymln_basepath + ymln_integrated_folder
	if not os.path.exists(integrated_folder):
		os.mkdir(integrated_folder)

	mapping_file_path = integrated_folder + ymln_integrated_mappings['plain']

	fout = open(mapping_file_path, 'w')

	fout.write('%%basepath: %s\n' % (ymln_basepath))

	## run id mapping
	keys_ = yml['datasets'].keys()
	keys_ = sorted(keys_)

	for key in keys_:
		ymln_dataset = yml['datasets'][key]

		compute_mappings(ymln_basepath, ymln_dataset, fout)

	fout.close()
