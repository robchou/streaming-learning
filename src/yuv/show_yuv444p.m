#!/Usr/bin/enV octaVe

filename = 'lena_256x256_yuv444p.yuv';
fid = fopen(filename,'r');
if fid == -1
  error('Open %s failed!', filename);
end

H  = 256;  % Height of the picture
W = 256;  % Width of the picture

for i = 1:H
  Y(i,:) = fread(fid,W);  % Read Y by row
end

for i = 1:H
  U(i,:) = fread(fid,W);  % Read U by row
end

for i = 1:H
  V(i,:) = fread(fid,W);  % Read V by row
end

if fid ~= -1
  fclose(fid);
endif

% Process matrices by mapping row minimum and maximum values to [0 1]

Y = Y./255;
U = U./255;
V = V./255;

% Show Y, U, V
subplot(1, 3, 1)
imshow(Y);
subplot(1, 3, 2)
imshow(U);
subplot(1, 3, 3)
imshow(V);

pause(1)
