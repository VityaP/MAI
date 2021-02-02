clear;
clc;

phi = 0 : 0.025 : 2 * pi;
x = secondFunc(phi);
xseq = con2seq(x);

net = feedforwardnet([10, 1, 10], 'trainlm');
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'tansig';
net.layers{3}.transferFcn = 'tansig';
net.layers{4}.transferFcn = 'purelin';
net = configure(net, xseq, xseq);
net = init(net);
view(net);

net.trainParam.epochs = 2000;
net.trainParam.goal = 10e-5;
net = train(net, xseq, xseq);

yseq = sim(net, xseq);
y = cell2mat(yseq);

plot(x(1, :), x(2, :), '-r', y(1, :), y(2, :), '-b', 'LineWidth', 2);