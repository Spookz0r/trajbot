close all, clear all, clc

DATA = dlmread('task13.cvs');
DATA2 = dlmread('task11_serial.cvs');

angle1 = DATA(:,2);
time1 = DATA(:,1);

plot(time1,angle1)
% time2 = DATA2(:,1);
% d0 = DATA2(:,2);
% thetaR = DATA2(:,3);
% subplot(1,2,1)
% plot(time2,d0)
% title('d0')
% subplot(1,2,2)
% plot(time2,thetaR)
% title('Theta R')