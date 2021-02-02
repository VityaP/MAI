clear;
clc;

P = [ 1.4  1.4    0 -0.3 -1.3 -1.4  0.9 0.3  1.1   -1   1.1    0;
      0.2  0.1 -0.6 -0.1  1.1 -0.2 -0.4 0.9  1.2 -0.8   0.2  0.3];

T = [1 1 1 -1 1 1 1 -1 1 1 1 -1];

plotpv(P, max(0, T));

Ti = T;
Ti(Ti == 1) = 2;
Ti(Ti == -1) = 1;
Ti = ind2vec(Ti);

portion = [nnz(T(T == -1)) nnz(T(T == 1))] / numel(T);
net = lvqnet(12, 0.1);
net = configure(net, P, Ti);
view(net)

net.IW{1,1}
net.LW{2,1}

net.trainParam.epochs = 300;
net = train(net, P, Ti);

[X,Y] = meshgrid([-1.5 : 0.1 : 1.5], [-1.5 : 0.1 : 1.5]);

res = sim(net, [X(:)'; Y(:)']);
res = vec2ind(res) - 1;

figure;
plotpv([X(:)'; Y(:)'], res);
point = findobj(gca,'type','line');
set(point,'Color','g');
hold on;
plotpv(P, max(0, T));