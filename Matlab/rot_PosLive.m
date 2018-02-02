function [X,Y] = rot_PosLive(xin, yin, w)
        %Euler

%theta = atan(yin/xin);

h = 1;
r = sqrt(xin^2+yin^2);



if xin > 0
   theta = atan(yin/xin);
    else if xin < 0 && yin >= 0 
          theta = atan(yin/xin) + pi;
        else if xin < 0 && yin <0
                theta = atan(yin/xin) - pi;
             else if xin == 0 && yin >0
                theta = pi/2;
                else if xin == 0 && yin <0
                theta = -pi/2;
                    end
                 end
            end
    end
end


  
thetaN = theta + h * w;

X = r*cos(thetaN);  
Y = r*sin(thetaN);




end