clear;
close all;

% Global variables
files (1,:) = '../logs/log_repair.txt  ';
files (2,:) = '../logs/log_norepair.txt';
outputname = './images/graph_bonus2.png';
colors = [[0.3020 0.7451 0.9333];[0.9294 0.6941 0.1255]];

fig = figure;
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(char(files(i,:)));
    runData = [];
    run = 1;
    eval = 1;
    best = 0;

    % Skip first 3 lines
    for j=1:3
        line = fgetl(file);
    end

    % Read in data
    while 1
        line = fgetl(file);
        if ~ischar(line)
            break
        end
        if(strncmpi(line, 'Run ', 4))
            run = run + 1;
            eval = 1;
            runData(run) = best;
            best = 0;
            continue
        end
        if(~isempty(line))
            lineData = textscan(line,'%f %f %f');
            lineData = cell2mat(lineData);
            if(lineData(3) > best)
                best = lineData(3);
                idx = lineData(1);
            end
            eval = eval + 1;
        end
    end
    fclose(file);
    
    % Graph data
    average = mean(runData);
    plot(1:1:length(runData),runData,'DisplayName',char(files(i,:)),'color',colors(i,:));
    p = plot(1:1:length(runData),repmat(average,1,length(runData)),'--','color',colors(i,:));
    hasbehavior(p,'legend',false);
    text(2,average+1,sprintf('average=%.1f',average),'FontWeight','bold');
end

% Format graph
hold off;
title('Best Solution Found per Run');
xlabel('Run');
ylabel('Fitness');
l = legend('show');
set(l, 'Interpreter', 'none');
saveas(fig, outputname);
