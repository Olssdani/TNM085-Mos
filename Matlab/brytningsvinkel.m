function [height,out_angle] = brytningsvinkel(angle,length, n1,n2)
    angle_medium =snells(n1,n2, angle);
    
    height = length * sin(angle_medium);
    
    out_angle = snells(n2,n1, angle_medium);
end

