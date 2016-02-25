close all, clear all, clc

data = dlmread('Forward.cvs');
time = data(:,1);
x = data(:,2);
y = data(:,3);
theta = data(:,4);

u_omega = 200;
u_psi = 0;

