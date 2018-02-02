function [X,Y] = rot_Pos(x, y, w)
        %Euler
theta = atan(y/x);

h = 0.1;

r = sqrt(x^2+y^2);

Y = zeros(1,1000);
X = zeros(1,1000);

thetaN = zeros(1, 1000);

    for t = 1:1000
  
        thetaN(t) = theta + h * w;

        X(t) = r*cos(thetaN(t));  
        Y(t) = r*sin(thetaN(t));

        theta = thetaN(t);

        drawnow
        axis equal
        plot(X, Y);

    end     
end