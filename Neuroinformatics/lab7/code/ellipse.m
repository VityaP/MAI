function [ o ] = ellipse( t, a, b, ShiftX, ShiftY, alpha )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
x1 = a * cos(t); 
y1 = b * sin(t);
o = [x1 * cos(alpha) - y1 * sin(alpha) + ShiftX;
     x1 * sin(alpha) + y1 * cos(alpha) + ShiftY];
end

