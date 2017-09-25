clear;
close all;

% Global variables
assn1a_files = [];
assn1b_files = [];
assn1a_files(1,:) = '../logs/assn1a_log_instance1.txt';
assn1b_files(1,:) = '../logs/log_instance1.txt';
%files (2,:) = '../logs/log_instance2.txt';
%files (3,:) = '../logs/log_instance3.txt';

for i=1:size(assn1a_files,1)

    % Import assn1a data
    data = importdata(char(assn1a_files(i,:)));
    assn1a_data = data.data;
    
    % Import assn1b data
    run = 1;
    eval = 1;
    file = fopen(char(assn1b_files(i,:)));
    for j=1:3
        line = fgetl(file);
    end
    while 1
        line = fgetl(file);
        if ~ischar(line)
            break
        end
        if(strncmpi(line, 'Run ', 4))
            run = run + 1;
            eval = 1;
            continue
        end
        if(~isempty(line))
            lineData = textscan(line,'%f %f %f');
            lineData = cell2mat(lineData);
            assn1b_data(run,eval) = lineData(2);
            eval = eval + 1;
        end
    end
    fclose(file);
    
    % Run f-test
    [h,p,ci,stats] = vartest2(assn1a_data(2,:),assn1b_data(2,:));
    [m,v] = fstat(assn1a_data(2,:),assn1b_data(2,:));

end