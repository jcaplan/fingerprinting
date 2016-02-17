filename = 'results.txt';
M = csvread(filename);
[m,n] = size(M);

S = sum(M,2);
C = zeros(m,n);
R = M > C;
bet = sum(R,2);
plot(bet)