function linux_grt_targetcallback(hDlg,hSrc,UN_target,IP_target,EX_target,CASE_target)
% *************************************************************************
% File:      linux_grt_target_go.m
% Purpose:   linux_grt_target_go.
% Project:   Linux Target
% Version:   1
% Author:    Pavel Jelinek
% Date:      2007/01/18
% 
%  Copyright:   
%  (c) 2007 Dept. of Control Engineering, FEE, CTU Prague
%   
% Description:
%   The function creates script for Cygwin, which compiles generated code
%   and copies and executes compiled code.
%
%  Modifications:
%  --
%  
% *************************************************************************

disp('I AM CREATING SCRIPT FOR CYGWIN.');

%\\\ This part loads value of variables.  ///
UN_target = slConfigUIGetVal(hDlg, hSrc, UN_target);
IP_target = slConfigUIGetVal(hDlg, hSrc, IP_target);
EX_target = slConfigUIGetVal(hDlg, hSrc, EX_target);
CASE_target = slConfigUIGetVal(hDlg, hSrc, CASE_target);

%\\\ Create file 'go' ///
fid = fopen('go','w');

%\\\ Write heading of the script. ///
fprintf(fid,'#!/bin/bash\n');

%\\\ Write command for compilation of generated code. ///
fprintf(fid,'cd ModelNamelinux_grt_rtw/\n');
fwrite(fid,'./'); 
fprintf(fid,'ModelName.bat\n');
fprintf(fid,'cd ..\n');


%\\\ According to 'CASE_target' write  ///
switch lower(CASE_target)
    case 'compile_copy' 
        %\\\ scp ModelName UN_target@IP_target:~. ///
        %\\\ It copies file 'ModelName' to target root.  ///
        fwrite(fid,'scp '); 
        fwrite(fid,'ModelName');
        fwrite(fid,' ');
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fprintf(fid,':~\n');
    
    case 'compile_copy_execute'     
        %\\\ scp ModelName UN_target@IP_target:~.  ///
        %\\\ It copies file 'ModelName' to target root.  ///
        fwrite(fid,'scp '); 
        fwrite(fid,'ModelName');
        fwrite(fid,' ');
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fprintf(fid,':~\n');
    
        %\\\ ssh UN_target@IP_target ./ModelName EX_target. ///        
        %\\\ It executes 'ModelName' on target.  ///
        fwrite(fid,'ssh '); 
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fwrite(fid,' ./');
        fwrite(fid,'ModelName ');
        fprintf(fid,'%s\n',EX_target);
    otherwise 
end

fclose(fid);