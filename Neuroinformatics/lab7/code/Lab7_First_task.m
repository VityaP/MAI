clear;
clc;

t = 0 : 0.025 : 2 * pi;
x = ellipse(t, 0.5, 0.4, 0.3, -0.1, 0);
xseq = con2seq(x);

net = feedforwardnet(1, 'trainlm');
net.layers{1}.transferFcn = 'purelin';
net = configure(net, xseq, xseq);
net = init(net);
view(net);

net.trainParam.epochs = 100;
net.trainParam.goal = 10e-5;
net = train(net, xseq, xseq);

yseq = sim(net, xseq);
y = cell2mat(yseq);

plot(x(1, :), x(2, :), '-r', y(1, :), y(2, :), '-b', 'LineWidth', 2);
