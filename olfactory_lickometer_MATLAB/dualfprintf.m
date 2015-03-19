function dualfprintf(fid,message,varargin)
fprintf(fid,message,varargin{:});
fprintf(message,varargin{:});
end