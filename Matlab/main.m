%% Task 3
%Not Zeno 
close all; clear all; clc
sim('rot1')
response = simout.Data(:,1);
c_signal = simout.Data(:,2);
time = simout.Time;
plot(time, response, time, c_signal)
legend('rotation [\theta]', 'control signal')
xlabel('time [s]')
print ('.\images\task3_plot', '-dpng')

%%Task 4
%Zeno
close all; clear all; clc
sim('rot2')
response = simout.Data(:,1);
c_signal = simout.Data(:,2);
time = simout.Time;
plot(time, response, time, c_signal)
legend('rotation [\theta]', 'control signal')
xlabel('time [s]')
print ('.\images\task4_plot', '-dpng')

%%Task 5
%Zeno
close all; clear all; clc
sim('rot3')
response = simout.Data(:,1);
c_signal = simout.Data(:,2);
time = simout.Time;
plot(time, response, time, c_signal)
legend('rotation [\theta]', 'control signal')
xlabel('time [s]')
print ('.\images\task5_plot', '-dpng')
