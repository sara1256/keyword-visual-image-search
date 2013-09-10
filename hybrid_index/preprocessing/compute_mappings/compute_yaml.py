#!/usr/bin/env python3.3

import os
import sys
import yaml

def sort_files_by_name(folder_path):
	filenames = filter(lambda x: x, os.listdir(folder_path))
	ids = map(lambda x: int(os.path.splitext(x)[0]), filenames)
	ids = sorted(ids)

	return map(lambda x: str(x) + '.jpg', ids)

unique_id = 0

## YAML structure
'''- path:
    - base_folder: %s
- datasets:
    - dataset_000:
        - images_folder: %s
          vlads_folder: %s
          tags_stemmed_folder: %s
        - mappings:
            - unique_id: %s
              image_name: %s
              vlad_name: %s
              tags__stemmed_name: %s
'''

YAML_TEMPLATE_DATASET = '''    - %s:
        - images_folder: %s
          vlads_folder: %s
          tags_stemmed_folder: %s
        - mappings:
'''

YAML_TEMPLATE_MAPPING = '''            - unique_id: %s
              image_name: %s
              vlad_name: %s
              tags_stemmed_name: %s
'''

def compute_mappings(image_folder_path):
	global unique_id 

	## check validity of input arguments
	if not image_folder_path.endswith('/'):
		image_folder_path = image_folder_path + '/'

	if not os.path.isdir(image_folder_path):
		print('[Error] Image folder', image_folder_path, 'does not exist.')
		raise IOError

	## file walker
	sorted_image_names = sort_files_by_name(image_folder_path)

	yml_str = ''

	for image_name in sorted_image_names:
		vlad_name = image_name + '.vlad'
		tags_name = 'stemmed_tags' + image_name

		yml_str += YAML_TEMPLATE_MAPPING % (unique_id, image_name, vlad_name, tags_name)

		unique_id += 1

	return yml_str

if __name__ == '__main__':
	## check validity of input arguments
	if len(sys.argv) != 2:
		print( 'Usage: ./compute_all.py [setting_file]\n' )
		os._exit(0)

	## load a setting file (yaml)
	yml = yaml.load(open(sys.argv[1]))

	## ask to run image to unique_id mapping
	want_to_run = input('Do you want to proceed mapping image to unique id (uid) from ' + yml['dataset_name'] + ' dataset? [yes/no]: ')
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

	mapping_file_path = integrated_folder + ymln_integrated_mappings['yaml']

	yml_fout = open(mapping_file_path, 'w')

	yml_str = '- path:\n'
	yml_str += '    - base_folder: %s\n' % (yml['path']['base'])
	yml_str += '- datasets:\n'

	yml_fout.write(yml_str)

	## run id mapping
	keys_ = yml['datasets'].keys()
	keys_ = sorted(keys_)

	for key in keys_:
		meta = yml['datasets'][key]
		images_folder = meta['images_folder']
		vlads_folder = meta['vlads_folder']
		tags_stemmed_folder = meta['tags_stemmed_folder']

		if not os.path.exists(yml['path']['base'] + images_folder):
			print('[Error] Image folder', images_folder, 'does not exist.')
			raise IOError

		print( '\nProcessing', images_folder, 'folder.')

		yml_str = YAML_TEMPLATE_DATASET % (key, images_folder, vlads_folder, tags_stemmed_folder)
		yml_str += compute_mappings(yml['path']['base'] + images_folder)

		yml_fout.write(yml_str)


	yml_fout.close()
