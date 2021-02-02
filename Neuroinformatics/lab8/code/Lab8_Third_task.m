clear;
clc;

varFunc = @(k)sin(k.^2-6.*k - 2 * pi)/4;
commonFunc = @(y, u)y ./ (1 + y.^2) + u.^3;

tStart = 0;
tEnd = 10;
h = 0.01;
n = 1 + (tEnd - tStart) / h;

u = zeros(1, n);
y = zeros(1, n);
u(1) = varFunc(0);

for i = 2 : n
    t = tStart + (i - 1) * h;
    y(i) = commonFunc(y(i - 1), u(i - 1));
    u(i) = varFunc(t);
end


x = u;
D = 3;

trainInd = 1 : 700;
valInd = 701 : 900;
testInd = 901 : 997;

net = narxnet(1 : 3, 1 : 3, 10);
net.trainFcn = 'trainlm';
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'purelin';

net.divideFcn = 'divideind';
net.divideParam.trainInd = trainInd;
net.divideParam.valInd = valInd;
net.divideParam.testInd = testInd;
net = init(net);
view(net);

net.trainParam.epochs = 600;
net.trainParam.max_fail = 600;
net.trainParam.goal = 1.0e-8;

[Xs, Xi, Ai, Ts] = preparets(net, con2seq(x), {}, con2seq(y));
net = train(net, Xs, Ts, Xi, Ai);
Y = sim(net, Xs, Xi, Ai); 


figure;
hold on;
grid on;
plot(tStart : h : tEnd, x, '-b', tStart : h : tEnd, [x(1:D) cell2mat(Y)], '-r')
legend('Source', 'Predicted');


figure;
hold on;
grid on;
plot(tStart+D*h : h : tEnd, x(D+1:end) - cell2mat(Y), '-r')
legend('Train Error');


xValid = x( valInd( length(valInd) - (D - 1) : length(valInd) ) );
uValid = u( valInd( length(valInd) - (D - 1) : length(valInd) ) );
inpuT   = [con2seq( u(testInd)  ); con2seq( x(testInd) )];
delay = [con2seq( uValid ); con2seq(xValid)];
predictTest = sim(net, inpuT, delay, Ai);

figure;
hold on;
grid on;
plot(x(testInd), '-b')
plot(cell2mat(predictTest), '-r');
legend('Source', 'Predicted');

figure;
hold on;
grid on;
error = cell2mat(predictTest) - x(testInd);
plot(error, '-r');
legend('Test Error');