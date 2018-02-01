%% Denna funktion får in en prisma och ger en graf över hur ljusstrålen bryts.
%Argumenten är:
%n1-Brytningsindex för mediumet utanför prismat
%n2-Brytningsindex för prismat
%Prismat i en 2x4 matris där första raden är x pos och andra raden är y pos
function [] = prism(n1, n2,prisma)
%% Inställningar
% Strålens längd
X =-6:0.001:6;
% Ljusstrålen, denna är alltid samma
K = 0;
M = 0;
% Allokering av minne för den brutna ljusstrålen
Y_out = zeros(1,length(X));

%% Linjerna för de olika prisma väggarna
% Första väggen
K_1 = (prisma(2,1)-prisma(2,2))/(prisma(1,1)-prisma(1,2));
M_1 =prisma(2,1)-K_1*prisma(1,1);

% Andra väggen
K_2 = (prisma(2,2)-prisma(2,3))/(prisma(1,2)-prisma(1,3));
M_2 =prisma(2,2)-K_2*prisma(1,2);

% Tredje väggen
K_3 = (prisma(2,3)-prisma(2,4))/(prisma(1,3)-prisma(1,4));
M_3 =prisma(2,3)-K_3*prisma(1,3);

%% Normaler och vektorer
% Ljus vecktorn
ljus = [1,0];

% Första väggens normalen
norm1 = [K_1,1];

% Första väggens normalen
norm2 = [K_2, 1];

% Första väggens normalen
norm3 = [K_3, 1];

%% Brytningsloopen
% Denna söker efter vilket vägg som strålen prickar och sedan plockar
% dennes normal och hittar den nya vinkeln.
index = 1;
Wall1 = true;
Wall2 = true;
Wall3 = true;
first = true;

% loopar över alla x värden
for a = X
    % Beräknar Y värden mot väggarna
    Y = K*a +M; % Ljustrålens y värde
    if a>=min(prisma(1,:)) && a<=max(prisma(1,:))
        Y1 = (K_1*a+M_1); % Första väggen
        Y2 = (K_2*a+M_2);% Andra väggen
        Y3 = (K_3*a+M_3); % Tredje väggen
        
        % Kollar om den krockar med någon väg
        if Wall1 && abs(Y - Y1) < 0.01
            % adderar talet
            Y_out(index) = Y;
            % Hittar vinkeln
            
          
            % Kollar om strålen är i mediumet eller inte
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

            % Kollar om strålen är i mediumet eller inte
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

            % Kollar om strålen är i mediumet eller inte
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

