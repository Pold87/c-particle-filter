%#codegen

function [model ] = mypredict_func( )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

% Read in histograms and x, y-coordinates
histograms = csvread('/home/pold/Documents/Internship/treXton/camel.csv', 0, 0);
targets = csvread('/home/pold/Documents/Internship/treXton/sift_targets_ortho.csv', 1, 1);

model = fitrsvm(histograms(1:100, :), targets(1:100, 1));

end

