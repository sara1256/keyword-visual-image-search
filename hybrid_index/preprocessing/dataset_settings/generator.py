#!/usr/bin/env python3.3

def define_dataset():
	rv = dict()

	rv['images_folder'] = input('images_folder: ')
	rv['siftgeos_folder'] = input('siftgeos_folder: ')
	rv['vlads_folder'] = input('vlads_folder: ')
	rv['tags_folder'] = input('tags_folder: ')
	rv['stemmed_tags_folder'] = input('stemmed_tags_folder: ')

	for key in rv.keys():
		if not rv[key].endswith('/'):
			rv[key] += '/'

	return rv

global_meta = dict()

print ('\n========== dataset_name : eg) mirflickr1M ==========')
global_meta['dataset_name'] = input('dataset_name: ')

print ('\n========== dataset_name : eg) /media/.../storage/mirflickr1M_dataset/working/ ==========')
global_meta['path_base'] = input('path.base: ')

print ('\n========== metadata for datasets ==========')
multiple = input('multiple datasets? [yes/no]: ')

if multiple == 'no':
	rv = define_dataset()
	save(global_meta, rv)

if multiple == 'yes':
	rv = define_dataset()

	from_no = int( input('Enter the start number of dataset: ') )
	end_no = int( input('Enter the end number of dataset: ') )

	yml = ''

	yml += 'dataset_name: %s\n' % ( global_meta['dataset_name'] )
	yml += 'path:\n'
	yml += ' base: %s\n' % ( global_meta['path_base'] )
	yml += 'datasets:\n'

	for k in range(from_no, end_no+1):
		sub_name = 'dataset_' + str(k).zfill(3)

		yml += ' %s:\n' % (sub_name)		
		yml += '  images_folder: %s%s/\n' % ( rv['images_folder'], str(k) )
		yml += '  siftgeos_folder: %s%s/\n' % ( rv['siftgeos_folder'], str(k) )
		yml += '  vlads_folder: %s%s/\n' % ( rv['vlads_folder'], str(k) )
		yml += '  tags_folder: %s%s/\n' % ( rv['tags_folder'], str(k) )
		yml += '  stemmed_tags_folder: %s%s/\n' % ( rv['stemmed_tags_folder'], str(k) )

	print (yml)

print ('\n========== output yaml filename ==========')
yml_filename = input('YAML filename: ')

with open(yml_filename, 'w') as f:
	f.write(yml)
