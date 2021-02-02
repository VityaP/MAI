P = [ 2   -2.3 -4.1  1.9  4.5  -0.7  2.6 -3.2; 
     -4.7 -4.6  3.2 -1.9 -4.7  -1.2  2.9 -0.2];
T = [1 1 0 1 1 1 0 1; 
     0 1 1 0 0 0 0 1];

net = newp([-5 5; -5 5], 2);
view(net);

net.inputWeights{1,1}.initFcn = 'rands';
net.biases{1}.initFcn         = 'rands';
net = init(net);
disp('Randomly generated Weights: ');
disp(net.IW{1,1});
disp('Randomly generated Biases : ');
disp(net.b{1});

net.trainParam.epochs = 50;
net = train(net, P, T);
disp('Weights: ');
disp(net.IW{1,1});
disp('Biases : ');
disp(net.b{1});

plotpv(P, T);
plotpc(net.IW{1}, net.b{1});
grid;

% In general, you can generate N random numbers
% in the interval (a,b) with the formula r = a + (b-a).*rand(N,1).
testpoints = -5 + (5+5)*rand(2,5);
testclasses = net(testpoints);
plotpv([P testpoints], [T, testclasses]); 
point = findobj(gca,'type','line');
set(point,'Color','red');
hold on
plotpv(P, T);
plotpc(net.IW{1}, net.b{1});
hold off