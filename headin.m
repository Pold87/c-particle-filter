% Prior
theta_0 = pi % initial heading
sigma2_0 = 1000 % initial variance is huge due to high uncertainty

% Likelihood
sigma2_X = 3; % 'Accuracy' of sensor

figure;
h_old = plot(0, 0);

for i=1:50;

% Measurements
N = 1; % Number of observations
X = 0.1 * i * pi; % Actual data %
    
% Posterior
total = N * X; % Total sum of measurements
theta_1 = (theta_0 / sigma2_0 + total / sigma2_X) / (1 / sigma2_0 + N / sigma2_X)

sigma2_1 = ((1 / sigma2_0) + (N / sigma2_X)) ^ (-1)
fprintf('Posterios sigma2_1 is %f', sigma2_1);

% Plot
theta = linspace(- 2 * pi, 2 * pi, 100);
Y_prior = normpdf(theta, theta_0, sqrt(sigma2_0));
Y_likelihood = normpdf(theta, X, sqrt(sigma2_X));
Y_posterior = normpdf(theta, theta_1, sqrt(sigma2_1));

h = plot(theta, Y_likelihood, '-', theta, Y_prior, '--', theta, Y_posterior, '-.')
delete(h_old);
h_old = h;
% hold on
legend('Likelihood', 'Prior', 'Posterior')

drawnow;
pause(1)


% Set posterios as new prior
theta_0 = theta_1;
sigma2_0 = sigma2_1;

end;

