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

net = distdelaynet({0 : 4, 0 : 4}, 8, 'trainoss');
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'tansig';
net.divideFcn = '';
net = configure(net, Pseq, Tseq);
view(net);

net.trainParam.epochs = 100;
net.trainParam.goal = 10e-5;

[Xs, Xi, Ai, Ts] = preparets(net, Pseq, Tseq); 
net = train(net, Xs, Ts, Xi, Ai);
Y = sim(net, Xs, Xi);

figure;
hold on;
grid on;
plot(cell2mat(Tseq), '-b');
plot([cell2mat(Xi) cell2mat(Y)], '-r');
legend('Source', 'Predicted');

Yc = zeros(1, numel(Xi) + numel(Y));
for i = 1 : numel(Xi)
    if Xi{i} >= 0
        Yc(i) = 1;
    else
        Yc(i) = -1;
    end
end
for i = numel(Xi) + 1 : numel(Y)
    if Y{i} >= 0
        Yc(i) = 1;
    else
        Yc(i) = -1;
    end
end

display( nnz(Yc == cell2mat(Tseq)) / length(Tseq) )


R = {8; 4; 2};
P = [repmat(p1, 1, R{1}), p2, repmat(p1, 1, R{2}), p2, repmat(p1, 1, R{3}), p2];
T = [repmat(t1, 1, R{1}), t2, repmat(t1, 1, R{2}), t2, repmat(t1, 1, R{3}), t2];

Pseq = con2seq(P);
Tseq = con2seq(T);

[Xs, Xi, Ai, Ts] = preparets(net, Pseq, Tseq); 

Y = sim(net, Xs, Xi);

figure;
hold on;
grid on;
plot(cell2mat(Tseq), '-b');
plot([cell2mat(Xi) cell2mat(Y)], '-r');
legend('Source', 'Predicted');

Yc = zeros(1, numel(Xi) + numel(Y));
for i = 1 : numel(Xi)
    if Xi{i} >= 0
        Yc(i) = 1;
    else
        Yc(i) = -1;
    end
end
for i = numel(Xi) + 1 : numel(Y)
    if Y{i} >= 0
        Yc(i) = 1;
    else
        Yc(i) = -1;
    end
end

display( nnz(Yc == cell2mat(Tseq)) / length(Tseq) )
