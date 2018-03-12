clear all
clc
%%
% Brytningsindex
n1 = 1.00029;
n2 = 1.51;

Prism_kord = [0, 0.34642, 0;
           0.3, -0.1732, 0;
          -0.3, -0.1732, 0;
          0,0.34642, 0]';  

%% Rotation
prism(n1,n2, Prism_kord);
%%
for i=0:1:1000

   
    for j = 1:1:4
     [X(j),Y(j)] = rot_PosLive(Prism_kord(1, j), Prism_kord(2, j), -pi/20); 
   end
    Roterad = [X ; Y ; 0 0 0 0];
    Prism_kord = [X ; Y ; 0 0 0 0];
    
    
    prism(n1,n2, Roterad);
   % prompt = '';
    %x = input(prompt)
    drawnow
    plot( X, Y);
    axis([-1 1 -1 1])
    xlim([-1 1])
    ylim([-1 1])
end

