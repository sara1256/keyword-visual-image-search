clear all;
clc;

addpath ('../../../3rd_party/matlab_yaml');

%----------------------------------------------------------------------------
% choose a dataset
fprintf('========== Merge vlads ==========\n');

disp('Which dataset do you want to merge VLADs?');
datasets = dir('../dataset_settings/*.yml');

while (true)
    for k=1:size(datasets, 1)
        disp([' ' num2str(k) ': ' datasets(k).name]);
    end
    disp(' 0: exit');
    
    which_dataset = input('Select: ');
    
    if which_dataset >= 0 && which_dataset <= size(datasets, 1)
        break;
    end
    
    if which_dataset == 0
        finish;
    end
end

%----------------------------------------------------------------------------
% traverse VLAD folders
yml = YAML.read(['../dataset_settings/' datasets(which_dataset).name]);

% widely-used yaml node
ymln_integrated = yml.path.integrated;

mapping_file_path = [yml.path.base ymln_integrated.base_folder ymln_integrated.mappings.plain];

disp(['Loading mapping file ' ymln_integrated.mappings.plain '...']);
[unique_ids, ~, vlad_paths, ~]  = textread(mapping_file_path, '%d\t%s\t%s\t%s', 'headerlines', 1);
disp('done.\n');

vladpcas = zeros(0,0);

for k=1:size(vlad_paths,1)
	vlad_path = [char(yml.path.base) char(vlad_paths(k)) 'pca'];

	vp_feature = load(vlad_path, '-mat');
    vladpca = vp_feature.vp;
    
    % resize vladpcas
    if k == 1
        nrows = size(unique_ids,1);
        ncols = size(vladpca,1);
        vladpcas = zeros(nrows, ncols);
    end
   
    vladpcas(k,:) = vladpca;
    
    if mod(k, 500) == 0
        fprintf('\t%d vladpca files are processed.\n', k);
    end
    
    fclose('all');
end

disp('Saving hdf5 file...');
output_path = [yml.path.base ymln_integrated.base_folder ymln_integrated.vlads];
hdf5write(output_path, 'vladpcas', vladpcas);
disp('done.\n');