function [out_angle] = snells(n1, n2, angle)
out_angle = asind(n1/n2*sind(angle));
end

