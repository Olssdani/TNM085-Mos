clear all
clc
% Brytningsindex
n1 = 1.00029;
n2 = 1.51;
Prism_kord = [0, 1.5, -1.5, 0;
    sqrt(6.75)/2, -sqrt(6.75)/2, -sqrt(6.75)/2, sqrt(6.75)/2];
%Prism_kord = [0, sqrt(6.75)/2;
 %           1.5, -sqrt(6.75)/2;
  %          -1.5, -sqrt(6.75)/2
   %         0,sqrt(6.75)/2];
vinkel=40 ;
%rotation = [cosd(vinkel) -sind(vinkel); sind(vinkel) cosd(vinkel)] 
%Prism_kord = Prism_kord*rotation;
%plot(Prism_kord(1,:),Prism_kord(2,:));
%axis([-6 6 -6 6]);
%grid on
prism(n1,n2, Prism_kord);