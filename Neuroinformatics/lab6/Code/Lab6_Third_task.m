clear;
clc;

T = -1.5 * ones(2, 20) + 3 * rand(2, 20);

figure;
hold on;
grid on;
plot(T(1,:), T(2,:), '-V', 'MarkerEdgeColor', 'k', 'MarkerFaceColor', 'g', 'MarkerSize', 7);

net = newsom(T, 20);
net = configure(net, T);
view(net);

net.trainParam.epochs = 600;
net = train(net, T);

figure;
hold on;
grid on;
plotsom(net.IW{1,1}, net.layers{1}.distances);
plot(T(1,:), T(2,:), 'V', 'MarkerEdgeColor', 'k', 'MarkerFaceColor', 'g', 'MarkerSize', 7);