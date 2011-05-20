% Copyright 2011 Zdenek Kalal
%
% This file is part of TLD.
% 
% TLD is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% TLD is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with TLD.  If not, see <http://www.gnu.org/licenses/>.


function img = img_get(source,I)

if source.camera
    if (exist('videoinput','file')) % using matlab with Image Acquisition package
        img = img_alloc(getsnapshot(source.vid));
    else % We don't have the Image Acquisition package, using stream_server
	if exist('OCTAVE_VERSION','builtin') % from octave
            [ data, count ] = recv (source.socket, 640*480, MSG_WAITALL);
            stream_img = transpose(data);
        else %from Matlab
            stream_img = transpose(fread(source.socket, [640, 480], 'uint8'));
        end 
        img = img_alloc(stream_img);
    end
else
    img = img_alloc(source.files(source.idx(I)).name);
end
