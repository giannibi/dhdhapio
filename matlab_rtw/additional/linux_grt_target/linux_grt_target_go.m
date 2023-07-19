function linux_grt_target_go(CANopen_support, UN_target, IP_target, EX_target, CASE_target, Model_name)
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
%   The function creates script for Cygwin, which compiles generated code and
%   copies and executes compiled code. 
%  Modifications:
%  --
%  
% *************************************************************************


%\\\ This part loads value of variables.  ///
% UN_target = slConfigUIGetVal(hDlg, hSrc, UN_target);
% IP_target = slConfigUIGetVal(hDlg, hSrc, IP_target);
% EX_target = slConfigUIGetVal(hDlg, hSrc, EX_target);
% CASE_target = slConfigUIGetVal(hDlg, hSrc, CASE_target);

%\\\ Create file 'go' ///
fid = fopen('../go','w');

%\\\ Write heading of the script. ///
fprintf(fid,'#!/bin/sh\n');

%\\\ Write command for compilation of generated code. ///
fwrite(fid,'cd ');
fwrite(fid,Model_name);
fprintf(fid,'linux_grt_rtw/\n');
fwrite(fid,'./'); 
fwrite(fid,Model_name);
fprintf(fid,'.bat\n');
fprintf(fid,'cd ..\n');

 %fwrite(fid,'chmod 777 ');
 %fwrite(fid,Model_name);
 %fprintf(fid,'\n');
        
%\\\ According to 'CASE_target' write  ///
switch lower(CASE_target)
    case 'compile_copy' 
        %\\\ scp ModelName UN_target@IP_target:~. ///
        %\\\ It copies file 'ModelName' to target home folder.  ///
        fwrite(fid,'scp '); 
        fwrite(fid,Model_name);
        fwrite(fid,' ');
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fprintf(fid,':~\n');
		
		if strcmpi(CANopen_support, 'on')
			fwrite(fid,'scp '); 
			fwrite(fid,Model_name);
			fwrite(fid,'linux_grt_rtw/canfestival/lib/libcanfestival_can_socket.so '); 
			fwrite(fid,UN_target);
			fwrite(fid,'@');
			fwrite(fid,IP_target);
			fprintf(fid,':/lib\n');
		end
    
    case 'compile_copy_execute'     
        %\\\ scp ModelName UN_target@IP_target:~.  ///
        %\\\ It copies file 'ModelName' to target root.  ///
        fwrite(fid,'scp '); 
        fwrite(fid,Model_name);
        fwrite(fid,' ');
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fprintf(fid,':~\n');
		
		if strcmpi(CANopen_support, 'on')
			fwrite(fid,'scp '); 
			fwrite(fid,Model_name);
			fwrite(fid,'linux_grt_rtw/canfestival/lib/libcanfestival_can_socket.so '); 
			fwrite(fid,UN_target);
			fwrite(fid,'@');
			fwrite(fid,IP_target);
			fprintf(fid,':/lib\n');
		end

        fwrite(fid,'ssh '); 
        fwrite(fid,UN_target);
        fwrite(fid,'@');
        fwrite(fid,IP_target);
        fwrite(fid,' ./');
        fwrite(fid,Model_name);
        fprintf(fid,' %s\n',EX_target);
    otherwise 
end

fclose(fid);

disp('### Script go for Linux emulator was created.');