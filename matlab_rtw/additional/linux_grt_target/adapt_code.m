function adapt_code(file,repl_what,repl_with)
% *************************************************************************
% File:      adapt_code.m
% Purpose:   Replaces string 'repl_what' with string 'repl_with' in 'file'.
% Project:   Linux Target
% Version:   1
% Author:    Pavel Jelinek
% Date:      2007/01/18
% 
%  Copyright:   
%  (c) 2007 Dept. of Control Engineering, FEE, CTU Prague
%   
% Description:
%  
%  Modifications:
%  --
%  
% *************************************************************************

disp(['### Adapting code from file ' file]);

%\\\ Open 'file' for reading and writing.  ///
fid = fopen(file,'r+'); 
%\\\ Open  temporary file for writing.  ///
fid1 = fopen([file,'1'],'w');


%\\\ Copy 'fid' to 'fid1' with modifications.  ///
while feof(fid) == 0
   line_string = fgets(fid);
   
   line_string=strrep(line_string,repl_what,repl_with);
   fwrite(fid1,line_string); 
end

fclose(fid);
fclose(fid1);

%\\\ Update 'file' and delete temporary file.  ///
delete(file);
copyfile([file,'1'],file);
delete([file,'1']);