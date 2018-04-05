
% The Fourier transform lets us express a function as a countable sum of "fourier coefficients".
% This demonstrates the discrete fourier transform: https://en.wikipedia.org/wiki/Discrete_Fourier_transform

% Some set up.
clear;
cla;
hold on;
axis equal;
axis square;
% Ignore this warning. imaginary parts will be small for real functions.
warning('off','MATLAB:plot:IgnoreImaginaryXYPart')


N = 128;						% Sample density
X = linspace(0, 2*pi, N);		% Interval length is 2*pi

% Our function. Experiment with this! Try X.^2, X.^10, etc.
Y = X;


% First we find the fourier coefficients, f_hat
f_hat = zeros(1,N);

% For each coeff, integrate f*exp(- ~~~)
for n=1:N
	x = 0:N-1;
	f_hat(n) = f_hat(n) + sum( Y .* exp(-(2*pi/N)*i*(n-1)*x) );
	f_hat(n) = f_hat(n)/N;
end

% Uncomment to crosscheck with MATLAB's fast fourier transform.
% f_hat = fft(Y).*(1/N);

% plot the scaled basis functions.
for n=1:N
	x = 0:N-1;
	plot(X, f_hat(n) .* exp(2*pi/N*i*(n-1)*x));	
end

% Reconstruct F by summing the fourier coefficients with basis functions.
F = zeros(1, N);
for x=1:N
	n = 0:N-1;
	F(x) = F(x) + sum(f_hat .* exp((2*pi/N)*i*(x-1).* n));
end

plot(X, F, 'LineWidth',3);
