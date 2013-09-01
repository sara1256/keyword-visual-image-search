clear all;
clc;

dir_inria_softwares = '../../../3rd_party/inria_softwares/';
dir_pqcodes = [dir_inria_softwares 'pqcodes_matlab/'];
dir_vlad = [dir_inria_softwares 'vlad_v1845/'];
dir_yael = [dir_inria_softwares 'yael_v300/'];
dir_data = [dir_inria_softwares 'vlad_v1845/data/'];

addpath ([dir_yael '/matlab']);
addpath (dir_pqcodes);
addpath (dir_vlad);
addpath ('../../../3rd_party/matlab_yaml');

f_centroids = [dir_data 'clust_k64.fvecs'];     % given in the package
f_vlad = [dir_data 'vlad_k64_holidays.fvecs'];  % computed
f_pca_proj = [dir_data 'pca_proj_matrix_vladk64_flickr1Mstar.fvecs']; % proj matrix

% Parameters
dd = 128;       % number of components kept by PCA transform
d_vlad = 8192;

%----------------------------------------------------------------------------
% load centroids for compute VLADs
centroids = fvecs_read(f_centroids);

%----------------------------------------------------------------------------
% VLAD with PCA projection
% perform the PCA projection, and keep dd components only
f = fopen(f_pca_proj);
mu = fvec_read (f);     % mean. Note that VLAD are already almost centered.
pca_proj = fvec_read (f);
pca_proj = reshape (pca_proj, d_vlad, 1024)';   % only the 1024 eigenvectors are stored
fclose(f);
pca_proj = pca_proj (1:dd,:);

%----------------------------------------------------------------------------
% choose a dataset
fprintf('========== Compute VLADS ==========\n');

disp('Which dataset do you want to extract VLADs ?');
datasets = dir('../dataset_settings/*.yml');

while (true)
    for k=1:size(datasets,1)
        disp([' ' num2str(k) ': ' datasets(k).name]);
    end
    disp(' 0: exit');

    which_dataset = input('Select: ');
    
    if which_dataset >= 0 && which_dataset <= size(datasets,1)
        break;
    end
    
    if which_dataset == 0
        finish;
    end
end

%----------------------------------------------------------------------------
% extract VLADs
yml = YAML.read(['../dataset_settings/' datasets(which_dataset).name]);

sub_datasets = fieldnames(yml.datasets);

for k=1:size(sub_datasets,1)
    obj = yml.datasets.( char(sub_datasets(k)) );
    
    disp( ['Processing ' obj.siftgeos_folder '...'] );
    
    dir_vlads_ = [yml.path.base obj.vlads_folder];
    dir_siftgeos_ = [yml.path.base obj.siftgeos_folder];
    
    if exist(dir_vlads_, 'dir') == 0
        mkdir( dir_vlads_ )
    end
    
    file_list = dir( [dir_siftgeos_ '*.siftgeo'] );
    
    for m=1:length(file_list)
        fname_siftgeo = [dir_siftgeos_ file_list(m).name];
        
        compute_and_store(centroids, pca_proj, mu, fname_siftgeo, dir_vlads_);
    end
end