%% Denna funktion f�r in en prisma och ger en graf �ver hur ljusstr�len bryts.
%Argumenten �r:
%n1-Brytningsindex f�r mediumet utanf�r prismat
%n2-Brytningsindex f�r prismat
%Prismat i en 2x4 matris d�r f�rsta raden �r x pos och andra raden �r y pos
function [] = prism(n1, n2,prisma)
%% Inst�llningar
% Str�lens l�ngd
X =-6:0.001:6;
% Ljusstr�len, denna �r alltid samma
K = 0;
M = 0;
% Allokering av minne f�r den brutna ljusstr�len
Y_out = zeros(1,length(X));

%% Linjerna f�r de olika prisma v�ggarna
% F�rsta v�ggen
K_1 = (prisma(2,1)-prisma(2,2))/(prisma(1,1)-prisma(1,2));
M_1 =prisma(2,1)-K_1*prisma(1,1);

% Andra v�ggen
K_2 = (prisma(2,2)-prisma(2,3))/(prisma(1,2)-prisma(1,3));
M_2 =prisma(2,2)-K_2*prisma(1,2);

% Tredje v�ggen
K_3 = (prisma(2,3)-prisma(2,4))/(prisma(1,3)-prisma(1,4));
M_3 =prisma(2,3)-K_3*prisma(1,3);

%% Normaler och vektorer
% Ljus vecktorn
ljus = [1,0];

% F�rsta v�ggens normalen
norm1 = [K_1,1];

% F�rsta v�ggens normalen
norm2 = [K_2, 1];

% F�rsta v�ggens normalen
norm3 = [K_3, 1];

%% Brytningsloopen
% Denna s�ker efter vilket v�gg som str�len prickar och sedan plockar
% dennes normal och hittar den nya vinkeln.
index = 1;
Wall1 = true;
Wall2 = true;
Wall3 = true;
first = true;

% loopar �ver alla x v�rden
for a = X
    % Ber�knar Y v�rden mot v�ggarna
    Y = K*a +M; % Ljustr�lens y v�rde
    if a>=min(prisma(1,:)) && a<=max(prisma(1,:))
        Y1 = (K_1*a+M_1); % F�rsta v�ggen
        Y2 = (K_2*a+M_2);% Andra v�ggen
        Y3 = (K_3*a+M_3); % Tredje v�ggen
        
        % Kollar om den krockar med n�gon v�g
        if Wall1 && abs(Y - Y1) < 0.01
            % adderar talet
            Y_out(index) = Y;
            % Hittar vinkeln
            
          
            % Kollar om str�len �r i mediumet eller inte
            if first
                angle_in = acosd(dot(ljus, norm1)/(norm(ljus)*norm(norm1)));
                
                if angle_in > 90
                    angle_in = 180 -angle_in;
                end
                [K, M, angle_out] = line_ekv(angle_in, Y, a, n1, n2);
                first = false;
            else
                angle_in = angle_in+(angle_in-angle_out);
                [K, M, angle_out] = line_ekv(angle_in, Y, a, n2, n1);
            end
           
            
            ljus =[1, K];
            
            Wall1 = false;
        elseif Wall2 && abs(Y - Y2) < 0.01 
            Y_out(index) = Y;
            angle_in = acosd(dot(ljus, norm2)/(norm(ljus)*norm(norm2)));
            if angle_in > 90
                angle_in = 180 -angle_in;
            end

            % Kollar om str�len �r i mediumet eller inte
            if first
                [K, M] = line_ekv(angle_in, Y, a, n1, n2);
                first = false;
            else
                [K, M,angle_out] = line_ekv(angle_in, Y, a, n2, n1);
            end
            
            ljus =[1, K]
            Wall2 = false;
        elseif Wall3 && abs(Y - Y3) < 0.01
            % adderar talet
            Y_out(index) = Y;
            % Hittar vinkeln
            angle_in = acosd(dot(ljus, norm3)/(norm(ljus)*norm(norm3)));
            if angle_in > 90
                angle_in = 180 -angle_in;
            end

            % Kollar om str�len �r i mediumet eller inte
            if first
                [K, M, angle_out] = line_ekv(angle_in, Y, a, n1, n2);
                first = false;
            else
                [K, M, angle_out] = line_ekv(angle_in, Y, a, n2, n1);
            end

            ljus =[1, K];
            Wall3 = false;
        else
            % addera talet
            Y_out(index) = Y;
        end
    else
        Y_out(index) = Y;
    end
    index = index + 1;
end


%% Plotning
plot(X,Y_out);
grid on
hold on
plot(prisma(1,:),prisma(2,:));
axis([-6 6 -6 6]);
hold off




end

