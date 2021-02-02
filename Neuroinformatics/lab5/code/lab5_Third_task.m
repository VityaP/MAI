clear;
clc;

target0 = [-1 -1 -1 -1 -1 -1 -1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 +1 +1 +1 +1 +1 +1 -1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 +1 +1 +1 -1 -1 +1 +1 +1 -1;
           -1 -1 +1 +1 +1 +1 +1 +1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 -1 -1 -1 -1 -1 -1 -1];
target1 = [-1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1;
           -1 -1 -1 +1 +1 +1 +1 -1 -1 -1];
target2 = [+1 +1 +1 +1 +1 +1 +1 +1 -1 -1;
           +1 +1 +1 +1 +1 +1 +1 +1 -1 -1;
           -1 -1 -1 -1 -1 -1 +1 +1 -1 -1;
           -1 -1 -1 -1 -1 -1 +1 +1 -1 -1;
           -1 -1 -1 -1 -1 -1 +1 +1 -1 -1;
           +1 +1 +1 +1 +1 +1 +1 +1 -1 -1;
           +1 +1 +1 +1 +1 +1 +1 +1 -1 -1;
           +1 +1 -1 -1 -1 -1 -1 -1 -1 -1;
           +1 +1 -1 -1 -1 -1 -1 -1 -1 -1;
           +1 +1 -1 -1 -1 -1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 +1 +1 -1 -1;
           +1 +1 +1 +1 +1 +1 +1 +1 -1 -1;];
target3 = [-1 -1 +1 +1 +1 +1 +1 +1 -1 -1;
           -1 -1 +1 +1 +1 +1 +1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 +1 +1 +1 +1 -1 -1;
           -1 -1 -1 -1 +1 +1 +1 +1 -1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 +1 +1 +1 +1 +1 +1 +1 -1;
           -1 -1 +1 +1 +1 +1 +1 +1 -1 -1];
target4 = [-1 +1 +1 -1 -1 -1 -1 +1 +1 -1;
           -1 +1 +1 -1 -1 -1 -1 +1 +1 -1;
           -1 +1 +1 -1 -1 -1 -1 +1 +1 -1;
           -1 +1 +1 -1 -1 -1 -1 +1 +1 -1;
           -1 +1 +1 -1 -1 -1 -1 +1 +1 -1;
           -1 +1 +1 +1 +1 +1 +1 +1 +1 -1;
           -1 +1 +1 +1 +1 +1 +1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1;
           -1 -1 -1 -1 -1 -1 -1 +1 +1 -1];
target6 = [+1 +1 +1 +1 +1 +1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 -1 -1 -1 -1;
           +1 +1 -1 -1 -1 -1 -1 -1 -1 -1;
           +1 +1 -1 -1 -1 -1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 -1 -1 -1 -1;
           +1 +1 -1 -1 +1 +1 -1 -1 -1 -1;
           +1 +1 -1 -1 +1 +1 -1 -1 -1 -1;
           +1 +1 -1 -1 +1 +1 -1 -1 -1 -1;
           +1 +1 -1 -1 +1 +1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 -1 -1 -1 -1;
           +1 +1 +1 +1 +1 +1 -1 -1 -1 -1];
target9 = [-1 -1 -1 -1 +1 +1 +1 +1 +1 +1;
           -1 -1 -1 -1 +1 +1 +1 +1 +1 +1;
           -1 -1 -1 -1 +1 +1 -1 -1 +1 +1;
           -1 -1 -1 -1 +1 +1 -1 -1 +1 +1;
           -1 -1 -1 -1 +1 +1 -1 -1 +1 +1;
           -1 -1 -1 -1 +1 +1 -1 -1 +1 +1;
           -1 -1 -1 -1 +1 +1 +1 +1 +1 +1;
           -1 -1 -1 -1 +1 +1 +1 +1 +1 +1;
           -1 -1 -1 -1 -1 -1 -1 -1 +1 +1;
           -1 -1 -1 -1 -1 -1 -1 -1 +1 +1;
           -1 -1 -1 -1 +1 +1 +1 +1 +1 +1;
           -1 -1 -1 -1 +1 +1 +1 +1 +1 +1];
       
%Print for check
        %res = reshape(target6(:), 12, 10);
        %res(res >=0 ) = 2;
        %res(res < 0 ) = 1;

        %map = [1, 1, 1; 0, 0, 0];
        %image(res);
        %colormap(map)
        %axis off
        %axis image

p = [target0(:), target1(:), target2(:), target3(:), target4(:), target6(:), target9(:)];
Q = 7;      %% Число эталонных образцов
R = 120;    %% Размерность входного вектора
eps = 1 / (Q - 1);
IW = [target0(:)';
      target1(:)';
      target2(:)';
      target3(:)';
      target4(:)';
      target6(:)';
      target9(:)'];
b = ones(Q, 1) * R;

a = zeros(Q, Q);
for i = 1 : Q
    a(:, i) = IW * p(:, i) + b;
end

net = newhop(a);
net.biasConnect(1) = 0;
net.layers{1}.transferFcn = 'poslin';
net.LW{1, 1} = eye(Q, Q) * (1 + eps) - ones(Q, Q) * eps;
view(net);

iterations = 600;
input = target4(:);
a1 = IW * input + b;
res = sim(net, {1 iterations}, {}, a1);
a2 = res{iterations};
ind = find(a2 == max(a2));
answer = IW(ind, :)';

res = reshape(answer, 12, 10);
res(res >=0 ) = 2;
res(res < 0 ) = 1;

map = [1, 1, 1; 0, 0, 0];
image(res);
colormap(map)
axis off
axis image

%% test 1
input = target2;
rando = rand([12, 10]);
noise_degree = 0.3;
for i = 1:12
    for j = 1:10
        if rando(i, j) < noise_degree
            input(i, j) = -input(i, j);
        end
    end
end
res = reshape(input, 12, 10);
res(res >=0 ) = 2;
res(res < 0 ) = 1;
map = [1, 1, 1; 0, 0, 0];
figure('Name', 'Noised');
image(res);
colormap(map)
axis off
axis image

input = input(:);
a1 = IW * input + b;
res = sim(net, {1 iterations}, {}, a1);
a2 = res{iterations};
ind = find(a2 == max(a2));
answer = IW(ind, :)';

figure
res = reshape(answer, 12, 10);
res(res >=0 ) = 2;
res(res < 0 ) = 1;
map = [1, 1, 1; 0, 0, 0];
image(res);
colormap(map)
axis off
axis image

%% test 2
input = target9;
rando = rand([12, 10]);
noise_degree = 0.3;
for i = 1:12
    for j = 1:10
        if rando(i, j) < noise_degree
            input(i, j) = -input(i, j);
        end
    end
end
res = reshape(input, 12, 10);
res(res >=0 ) = 2;
res(res < 0 ) = 1;
map = [1, 1, 1; 0, 0, 0];
figure('Name', 'Noised');
image(res);
colormap(map)
axis off
axis image

input = input(:);
a1 = IW * input + b;
res = sim(net, {1 iterations}, {}, a1);
a2 = res{iterations};
ind = find(a2 == max(a2));
answer = IW(ind, :)';

figure
res = reshape(answer, 12, 10);
res(res >=0 ) = 2;
res(res < 0 ) = 1;
map = [1, 1, 1; 0, 0, 0];
image(res);
colormap(map)
axis off
axis image
