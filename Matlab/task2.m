%% Task 2

close all; clear all; clc;

% Forward and Rotates first row is removed to be able to read. Probably not
% relevant.  %[t, x, y, theta]
indata = dlmread('Forward.cvs', ';');
forward_t = indata(:,1); % time
forward_x = indata(:,2);
forward_y = indata(:,3);
forward_theta = indata(:,4);

indata = dlmread('Rotate.cvs', ';');
rotate_t = indata(:,1);
rotate_x = indata(:,2);
rotate_y = indata(:,3);
rotate_theta = indata(:,4);
clear indata;

% Forward case :
% Calculate xdot through discrete derivate
uw = 200;
up = 0;


t1 = 0;
x1 = 0;
for i = 1:length(forward_t)
    t0 = forward_t(i);
    x0 = forward_x(i);
    xdot(i) = (x0-x1)/(t0-t1);
    R(i) = xdot(i)/(uw*cosd(forward_theta(i)))*10^6; %*10^6 conv from ms
    t1 = t0;
    x1 = x0;
end

% Calculate ydot through discrete derivate

t1 = 0;
y1 = 0;
Rlen = length(R);
for i = 1:length(forward_t)
    t0 = forward_t(i);
    y0 = forward_y(i);
    ydot(i) = (y0-y1)/(t0-t1);
    R(Rlen +i) = ydot(i)/(uw*sind(forward_theta(i)));
    t1 = t0;
    y1 = y0;
end

L = 0; %uPhi = 0


%plot(forward_t, xdot);
xdotmean = mean(xdot, 'omitnan');
ydotmean = mean(ydot, 'omitnan');
rmean = mean(R, 'omitnan');

fprintf ('In forward mode: \n')
fprintf ('Uw = %d\nUphi = %d\n', uw,up); 
fprintf ('Mean Xdot = %d\n', xdotmean)
fprintf ('Mean Ydot = %d\n', ydotmean)
fprintf ('R = %d [m]\n', rmean)
fprintf ('L = unknown\n', L)


% Rotate case :
% Using R from forward case to calculate L
clear x0 x1 t0 t1 L;

uw = 0;
up = 400;


t1 = 0;
th1 = 0;
for i = 1:length(rotate_t)
    t0 = rotate_t(i);
    th0 = rotate_theta(i);
    thetadot(i) = (th0-th1)/(t0-t1)*10^6;
    L(i) = -(rmean*up)/(thetadot(i));
    t1 = t0;
    th1 = th0;
end


rmean = mean(R, 'omitnan');
lmean = mean(L, 'omitnan');
fprintf ('\nIn rotate mode: \n')
fprintf ('Uw = %d\nUphi = %d\n', uw,up); 
fprintf ('Mean Xdot = %d\n', xdotmean)
fprintf ('Mean Ydot = %d\n', ydotmean)
fprintf ('R = %d [m]\n', rmean)
fprintf ('L = %d [m/deg]\n', lmean)

