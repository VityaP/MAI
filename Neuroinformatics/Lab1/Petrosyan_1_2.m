P = [-1 -1  1 1;
     -1  1 -1 1];
T =  [0  1  1 0];

net = newp([-5 5; -5 5], [0 1]); % построили сеть

net.inputWeights{1,1}.initFcn = 'rands';
net.biases{1}.initFcn = 'rands';
net = init(net);
disp('Randomly generated Weights: ');
disp(net.IW{1,1});
disp('Randomly generated Biases : ');
disp(net.b{1});

plotpv(P, T), grid
plotpc(net.IW{1,1}, net.b{1});
hold on;

net.trainParam.epochs = 50;
net = train(net, P, T);

plotpc(net.IW{1,1},net.b{1});
y = net(P);
mae(T - y)
hold off;


