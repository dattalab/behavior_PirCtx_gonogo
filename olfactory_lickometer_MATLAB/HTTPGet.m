function [res] = HTTPGet(url, map)
 
    if (nargin ~= 2) 
        ME = MException('argument:length',['Argument should be two, not enough argument. ']);
        ME.throw; 
    end
    c = [keys(map); values(map)];
    res = urlread(url,'Get',{c{:}})
 
end
