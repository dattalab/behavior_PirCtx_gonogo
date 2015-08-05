function dualfprintf(fid,message,varargin)
fprintf(fid,message,varargin{:});
fprintf(strrep(message,'\r',''),varargin{:});
end