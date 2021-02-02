clear;
clc;

%In the TXT and CSV files, the missing values are marked by -1 (valid Sunspot Number are always positive).

%Contents:
    %Column 1-3: Gregorian calendar date
        %- Year
        %- Month
        %- Day
    %Column 4: Date in fraction of year
    %Column 5: Daily total sunspot number. A value of -1 indicates that no number is available for that day (missing value).
    %Column 6: Daily standard deviation of the input sunspot numbers from individual stations.
    %Column 7: Number of observations used to compute the daily value.

data = load('mySunData.txt'); 
nums = data(:, 4);
x = smooth(nums, 12);

%x = con2seq(x(1:500 + 100 + 50)');
x = con2seq(x(1:650)');

D = 5;
net = timedelaynet(1:D, 8, 'trainlm');
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'purelin';
net.divideFcn = 'divideind';
net.divideParam.trainInd = 1 : 500;
net.divideParam.valInd = 501 : 600;
net.divideParam.testInd = 601 : 650;
net = configure(net, x, x);
net = init(net);

net.trainParam.epochs = 600;
net.trainParam.max_fail = 600;
net.trainParam.goal = 10e-5;
view(net);

%Prepare input and target time series data for network simulation or training
[Xs, Xi, Ai, Ts] = preparets(net, x, x);
% Xs Shifted inputs
% Xi Initial input delay states
% Ai Initial layer delay states
% Ts Shifted targets

net = train(net, Xs, Ts, Xi, Ai);
Y = sim(net, Xs, Xi);

figure;
hold on;
grid on;
plot(cell2mat(x), '-b');
plot([cell2mat(Xi), cell2mat(Y)], '-r');

figure;
hold on;
grid on;
error = cell2mat(x)- [cell2mat(Xi), cell2mat(Y)];
plot(error, '-g');

xCheck = cell2mat(x);
yCheck = cell2mat(Y);

figure;
hold on;
grid on;
plot(xCheck(601 : 650), '-b');
plot(yCheck(601 - D : 650 - D), '-r');

figure;
hold on;
grid on;
error = xCheck(601 : 650)- yCheck(601 - D : 650 - D);
plot(error, '-g');
