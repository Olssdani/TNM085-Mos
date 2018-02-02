function [K,M,angle_out] = line_ekv_second(angle,start_angle,angle_ref, Y, X, n1, n2)
        
            %Snells lag
            angle
            start_angle
            angle_ref
            angle_out = snells(n1,n2, angle)
            % hittar temporör punkt
            y_temp = Y- 0.1*tand(angle_out-60+start_angle);
            x_temp = X+0.1;
            % Räknar ut nya K och M värden
            K = (y_temp -Y)/(x_temp-X);
            M = Y-K*X;
       
end

