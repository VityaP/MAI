function [ o ] = secondFunc( alpha )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
r = 1 - 3 * (alpha .^ 2); 
o = [ r .* cos(alpha); r .* sin(alpha)];
end

