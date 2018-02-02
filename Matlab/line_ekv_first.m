function [K,M,angle_out] = line_ekv_first(angle, Y, X, n1, n2)
        
        if angle<60
            %Snells lag
            angle_out = snells(n1,n2, angle);
            % hittar temporör punkt
            y_temp = Y -0.1*tand(angle -angle_out);
            x_temp = X+0.1;
            % Räknar ut nya K och M värden
            K = (y_temp -Y)/(x_temp-X);
            M = Y-K*X;
        else
            %Snells lag
            angle_out = snells(n1,n2, angle);
            % hittar temporör punkt
            y_temp = Y +0.1*tand(180-angle -angle_out);
            x_temp = X+0.1;
            % Räknar ut nya K och M värden
            K = (y_temp -Y)/(x_temp-X);
            M = Y-K*X;
        end
end

