clear all
clc
%%
% Brytningsindex
n1 = 1.00029;
n2 = 1.51;
%Prism_kord = [0, 1.5, -1.5, 0;
 %   sqrt(6.75)/2, -sqrt(6.75)/2, -sqrt(6.75)/2, sqrt(6.75)/2];
% Prism_kord = [0, sqrt(6.75)/2, 1;
%            1.5, -sqrt(6.75)/2, 1;
%           -1.5, -sqrt(6.75)/2, 1;
%           0,sqrt(6.75)/2, 1]';
% Prism_kord = [0, 1.875, 1;
%            1.5, -1.1250, 1;
%           -1.5, -1.1250, 1;
%           0,1.875, 1]';  
      
Prism_kord = [0, 2, 1;
           2, -2, 1;
          -2, -2, 1;
          0,2, 1]';  
%% Rotation

for i=0:0.01:2*pi

    rotation = [cos(i) -sin(i) 0;
         sin(i) cos(i) 0;
         0 0 1]; 
    Roterad = rotation*Prism_kord;
    plot(Roterad(1,:),Roterad(2,:));
    prism(n1,n2, Roterad);
    axis([-6 6 -6 6]);
    grid on
    drawnow
    %pause(5)
   
end


% 
% 
% 
% 

% grid on
