close all, clear all, clc
realdata = dlmread('RealRobotLogfile');
realtime = realdata(:,1);
realx = realdata(:,2);
realy = realdata(:,3);
realangle = realdata(:,4);


simdata = dlmread('SimulationLogfile');
simtime = simdata(:,1);
simx = simdata(:,2);
simy = simdata(:,3);
simangle = simdata(:,4);
% PATH
figure
subplot(1,2,1)
plot(realx,realy)
title('Line following Robot')
ylabel('y')
xlabel('x')
subplot(1,2,2)
plot(simx,simy)
title('Line following Simulation')
ylabel('y')
xlabel('x')

%Angle 
figure
subplot(1,2,1)
plot(realtime,realangle)
title('Theta Robot')
ylabel('Theta')
xlabel('Angle')
subplot(1,2,2)
plot(simtime,simangle)
title('Theta Simulation')
ylabel('Theta')
xlabel('Time')

figure
plot(realtime,realangle,simtime,simangle)
