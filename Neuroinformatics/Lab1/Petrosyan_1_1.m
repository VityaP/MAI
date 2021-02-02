% Вариант 15 Петросян Виктор группа М80-408Б-17
%1.1
P = [-4.1 -1.7 -3.7 -4  -0.1 2.1;
     -2.4  1.7  2.2  1.5 2.7 4];
T = [1 1 0 0 1 1];

%1.2
net = newp([-5 5; -5 5], [0 1]); % построили сеть
%display(net); % информация о сети в консоль
view(net); % сеть на экран

%1.3.1
% инициализировали сеть случайными значениями
net.inputWeights{1,1}.initFcn = 'rands'; % весовые коэффициенты
net.biases{1}.initFcn = 'rands'; % смещения
net = init(net);
disp('Randomly generated Weights: ');
disp(net.IW{1,1});
disp('Randomly generated Biases : ');
disp(net.b{1});

plotpv(P, T); % рисуем точки
line1 = plotpc(net.IW{1}, net.b{1}); % рисуем прямую
set(line1, 'Color', 'r');
grid; % рисуем сетку
hold on;

%1.3.2
% Выполняем 2 итерации алгоритма Розенблатта
net = Rosenblatt(net, P, T, 2, 0.3);

%1.3.3
line2 = plotpc(net.IW{1}, net.b{1}); % рисуем прямую

%1.4.1
net = init(net);
disp('Randomly generated Weights: ');
disp(net.IW{1,1});
disp('Randomly generated Biases : ');
disp(net.b{1});

%1.4.2
% Выполним 50 итераций с помощью функции train
net.trainParam.epochs = 50;
net = train(net, P, T);

disp('After 50 iters Weights: ');
disp(net.IW{1,1});
disp('After 50 iters Biases : ');
disp(net.b{1});

%1.4.3
% создаем набор тестовых точек

% In general, you can generate N random numbers
% in the interval (a,b) with the formula r = a + (b-a).*rand(N,1).
testpoints = -5 + (5+5)*rand(2,3);
testclasses = net(testpoints);

% рисуем их
plotpv(testpoints, testclasses);
point = findobj(gca,'type','line');
set(point,'Color','red');
hold on
plotpv(P, T)
plotpc(net.IW{1},net.b{1});
grid on
hold off



