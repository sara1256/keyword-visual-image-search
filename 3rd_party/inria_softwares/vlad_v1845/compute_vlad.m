% Compute the set of VLAD descriptors for a set of images
% Usage: V = compute_vlad (centroids, S)
%
% where
%   centroids is the dictionary of centroids 
%   S         is a cell structure. Each cell is a set of descriptors for an image
%
% Both centroids and descriptors are stored per column
function V = compute_vlad (centroids, S)

nimg = length (S);
k = size (centroids, 2);
d = size (centroids, 1);
V = zeros (k * d, nimg, 'single');

for i = 1:nimg
  V(:, i) = vlad (centroids, S{i});
end
