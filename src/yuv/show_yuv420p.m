#!/usr/bin/env octave

filename = 'lena_256x256_yuv420p.yuv';
H = 256; % Height of picture
W = 256; % Width of picture

fid = fopen(filename, 'r');

Y = fread(fid, [H,W], 'uint8');
U = fread(fid, [H/2,W/2], 'uint8');
V = fread(fid, [H/2,W/2], 'uint8');

p = 1;
for i=1:H
  for j=1:W
    y(i,j)=Y(p);
    p=p+1;
  end
end

q=1;
for i=1:(H/2)
  for j=1:(W/2 )
    u(i,j)=U(q);
    q=q+1;
  end
end

t=1;
for i=1:(H/2)
  for j=1:(W/2 )
    v(i,j)=V(t);
    t=t+1;
  end
end

uu(1:2:H-1,1:2:W-1)=u(:,:);
uu(1:2:H-1,2:2:W)=u(:,:);
uu(2:2:H,1:2:W-1)=u(:,:);
uu(2:2:H,2:2:W)=u(:,:);

vv(1:2:H-1,1:2:W-1)=v(:,:);
vv(1:2:H-1,2:2:W)=v(:,:);
vv(2:2:H,1:2:W-1)=v(:,:);
vv(2:2:H,2:2:W)=v(:,:);

y = y./255;
uu = uu./255;
vv = vv./255;

subplot(1, 3, 1)
imshow(y);
title('Y');
subplot(1, 3, 2)
imshow(uu);
title('U');
subplot(1, 3, 2)
subplot(1, 3, 3)
imshow(vv);
title('V');
subplot(1, 3, 2)
pause(1)
