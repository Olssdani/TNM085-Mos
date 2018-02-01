%% inst�llningar
clear all
clc
% Brytningsindex
n1 = 1.00029;
n2 = 1.51;
% X v�rde
low = -6;
high = 6;
step = 0.001;
X =low:step:high;
% Inv�gen
K = 0;
M = 0;
% Allokering av minne
Y_out = zeros(1,length(X));

%% Linjerna
% F�rsta v�gen
XAK = (sqrt(6.75)/2-(-sqrt(6.75)/2))/(0-(-1.5));
XAM =sqrt(6.75)/2-XAK*0;

% Andra v�gen
XAK2 = (sqrt(6.75)/2-(-sqrt(6.75)/2))/(0-(1.5));
XAM2 =sqrt(6.75)/2-XAK2*0;


%% Vektorer
% Ljus vecktorn
ljus = [1,0];
% F�rsta normalen
vec = [XAK, 1];

%%
i = 1;
second = true;

% loopar �ver alla x v�rden 
for a = X
    % Ber�knar Y v�rden mot v�ggarna
    y1 = (XAK*a+XAM);
    y2 = (XAK2*a+XAM2);
    Y = K*a +M;
    % Kollar om den krockar med n�gon v�g
    if abs(Y - y1) < 0.001
        % addear talet
        Y_out(i) = Y;
        % Hittar vinkeln
        angle_in = acosd(dot(ljus, vec)/(norm(ljus)*norm(vec)));
        %Snells lag
        angle_out = snells(n1,n2, angle_in);
        % hittar tempor�r punkt
        y_temp = Y -0.1*tand(angle_in -angle_out);
        x_temp = a+0.1
        % R�knar ut nya K och M v�rden
        K = (y_temp -Y)/(x_temp-a);
        M = Y-K*a;     
        % Kollar n�sta v�g
    elseif abs(Y - y2) < 0.01 && second
          Y_out(i) = Y;
          angle_in =angle_in +(angle_in-angle_out)
          angle_out = snells(n2,n1, angle_in)
          y_temp = Y +0.1*tand(angle_in -angle_out);
          x_temp = a+0.1;
          K = (y_temp -Y)/(x_temp-a);
          M = Y-K*a;
          second = false;
    else  
        % addera talet
        Y_out(i) = Y;
    end
    i = i + 1;
end



plot(X,Y_out);
grid on

hold on
xT = [0, 1.5, -1.5, 0];
yT = [sqrt(6.75)/2, -sqrt(6.75)/2, -sqrt(6.75)/2, sqrt(6.75)/2];
plot(xT,yT);
axis([-6 6 -6 6]);