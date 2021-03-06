close all, clear all, clc

DATA = dlmread('task19.txt');
DATA2 = dlmread('task19_serial.txt');

angle1 = DATA(:,2);
time1 = DATA(:,1);

time2 = DATA2(:,1);

% plot(time1,angle1)
% ylabel('dp')
% xlabel('Time')

time2 = DATA2(:,1);
thetad= DATA2(:,2);
x0d= DATA2(:,3);
y0d= DATA2(:,4);
thetag= DATA2(:,5);
xg= DATA2(:,6);
yg= DATA2(:,7);
state= DATA2(:,8);


figure(88)


subplot(4,1,1)

plot(time2,x0d)
hold on
plot(time2,xg)
legend('Error X0','Error Xg')
ylabel('x')
grid on
title('Continuous and discrete trajectories')

subplot(4,1,2)
plot(time2,y0d)
hold on
plot(time2,yg)
legend('Error Y0','Error Yg')
ylabel('y')
grid on


subplot(4,1,3)
plot(time2,thetad)
hold on
plot(time2,thetag)
legend('Error theta0','Error thetag')
ylabel('theta')
grid on

subplot(4,1,4)
stairs(time2,state,'.b')
ylabel('State')
xlabel('Time')


% 
% figure(89)
% title('Title')
% subplot(4,1,1)
% plot(time2,xg)
% ylabel('x')
% xlabel('Time')
% 
% subplot(4,1,2)
% plot(time2,yg)
% ylabel('y')
% xlabel('Time')
% 
% subplot(4,1,3)
% plot(time2,thetag)
% ylabel('thetaR')
% xlabel('Time')
% 
% subplot(4,1,4)
% stairs(time2,state)
% ylabel('State')
% xlabel('Time')
% 




