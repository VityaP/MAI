clear;
clc;

k = 0 : 0.025 : 1;
p1 = sin(4 * pi * k);
t1 = -ones(size(k));

k = 0.92 : 0.025 : 3.25;
g = @(k) cos( -2 * k .^ 2 + 7 * k);
p2 = g(k);
t2 = ones(size(k));

R = {0; 4; 2};
P = [repmat(p1, 1, R{1}), p2, repmat(p1, 1, R{2}), p2, repmat(p1, 1, R{3}), p2];
T = [repmat(t1, 1, R{1}), t2, repmat(t1, 1, R{2}), t2, repmat(t1, 1, R{3}), t2];

Pseq = con2seq(P);
Tseq = con2seq(T);

net = layrecnet(1 : 2, 8, 'trainoss');
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'tansig';
net = configure(net, Pseq, Tseq);
view(net);

%% Prepare input and target time series data for network simulation or training
%% [Xs,Xi,Ai,Ts,EWs,shift] = preparets(net,Xnf,Tnf,Tf,EW)
[p, Pi, Ai, t] = preparets(net, Pseq, Tseq);

net.trainParam.epochs = 100;
net.trainParam.goal = 1.0e-5;
net = train(net, p, t, Pi, Ai);
Y = sim(net, p, Pi, Ai);

figure;
hold on;
plot(cell2mat(t), '-b');
plot(cell2mat(Y), '-r');
legend('Target', 'Output');

Yc = zeros(1, numel(Y));
for i = 1 : numel(Y)
    if Y{i} >= 0
        Yc(i) = 1;
    else
        Yc(i) = -1;
    end
end
%% nnz Number of nonzero matrix elements
display(nnz(Yc == T(3 : end)))


%% test
R = {8; 4; 2};
P = [repmat(p1, 1, R{1}), p2, repmat(p1, 1, R{2}), p2, repmat(p1, 1, R{3}), p2];
T = [repmat(t1, 1, R{1}), t2, repmat(t1, 1, R{2}), t2, repmat(t1, 1, R{3}), t2];
Pseq = con2seq(P);
Tseq = con2seq(T);
[p, Pi, Ai, t] = preparets(net, Pseq, Tseq);
Y = sim(net, p, Pi, Ai);
figure;
hold on;
plot(cell2mat(t), '-b');
plot(cell2mat(Y), '-r');
legend('Target', 'Output');