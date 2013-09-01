function compute_and_store( centroids, pca_proj, mu, input_path_sift, output_dir_vlad )

sift = cell (1, 1);

sift{1} = single (siftgeo_read_fast (input_path_sift));

v = compute_vlad (centroids, sift);
vn = yael_fvecs_normalize (v);

% project the descriptors and compute the results after PCA
vp = pca_proj * (v - repmat (mu, 1, size(v,2)));
vp = yael_fvecs_normalize (vp);

[~, name, ~] = fileparts(input_path_sift);

if strcmp(output_dir_vlad(end), '/') ~= 1
    output_dir_vlad = [output_dir_vlad '/'];
end

% save vlad and vladpca
save([output_dir_vlad name '.vlad'], 'vn');
save([output_dir_vlad name '.vladpca'], 'vp');

end

