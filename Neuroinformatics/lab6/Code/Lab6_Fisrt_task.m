clear;
clc;

X = [0, 1.5; 0, 1.5];
clusters = 8;
points = 10;
deviation = 0.1;

P = nngenc(X, clusters, points, deviation);

figure;
hold on;
grid on;
scatter(P(1, :), P(2, :), 5, [0 0 1], 'filled');

net = competlayer(8);
net = configure(net, P);
view(net);

net.trainParam.epochs = 50;
net = train(net, P);

RandomPoints = 1.5 * rand(2, 5);
res = vec2ind(sim(net, RandomPoints));

figure;
hold on;
grid on;
scatter(P(1, :), P(2, :), 5, [0 0 1], 'filled');
scatter(net.IW{1}(:, 1), net.IW{1}(:, 2), 5, [0 1 0], 'filled');
scatter(RandomPoints(1, :), RandomPoints(2, :), 5, [1 0 0], 'filled');