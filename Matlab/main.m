clear all
clc
%%
% Brytningsindex
n1 = 1.00029;
n2 = 1.51;

Prism_kord = [0, 2.5981-0.866, 0;
           1.5, -0.866, 0;
          -1.5, -0.866 0;
          0,2.5981-0.866, 0]';  

%% Rotation
% for i=0:1:360
%     rotation = [cosd(-i) -sind(-i) 0;
%         sind(-i) cosd(-i) 0;
%         0 0 1]; 
%     Roterad = rotation*Prism_kord;
%     prism(n1,n2, Roterad);
%     prompt = '';
%     x = input(prompt);
%     clc
%     drawnow
% end
    
    for j = 1:1:4
     [X(j),Y(j)] = rot_PosLive(Prism_kord(1, j), Prism_kord(2, j), -pi/20); 
   end
    Roterad = [X ; Y ; 0 0 0 0];
    Prism_kord = [X ; Y ; 0 0 0 0];
    
    
    %prism(n1,n2, Roterad);
   % prompt = '';
    %x = input(prompt)
    drawnow
    plot( X, Y);
    xlim([-5 5])
    ylim([-5 5])
end
