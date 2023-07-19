function sl_customization(cm)
    disp('Adding tcpip external mode to test target');
    cm.ExtModeTransports.add('linux_grt_target.tlc', 'tcpip', 'ext_comm', 'Level1');
end

