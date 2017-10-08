clear;
close all;

% Global variables
files(1,:) = '../logs/bonus2_control_set3.txt  ';
files(2,:) = '../logs/bonus2_crossover_set3.txt';
files(3,:) = '../logs/bonus2_mutation_set3.txt ';  
files(4,:) = '../logs/bonus2_both_set3.txt     ';  
outputname = './images/assn1c_bonus2_set3.png';
colors = [[0.3020 0.7451 0.9333];[0.9294 0.6941 0.1255];[0 0.6000 0.2000];[0.4941 0.1843 0.5569];[0.8510 0.3255 0.0980]];

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
            if(i==1)
                lineData = textscan(line,'%f %f %f');
                lineData = cell2mat(lineData);
                if(lineData(3) > best)
                    best = lineData(3);
                    idx = lineData(1);
                end
            elseif(i==4)
                lineData = textscan(line,'%f %f %f %f %f');
                lineData = cell2mat(lineData);
                if(lineData(4) > best)
                    best = lineData(5);
                    idx = lineData(1);
                end
            else
                lineData = textscan(line,'%f %f %f %f');
                lineData = cell2mat(lineData);
                if(lineData(4) > best)
                    best = lineData(4);
                    idx = lineData(1);
                end
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
%title('Best Solution Found per Run');
xlabel('Run');
ylabel('Fitness');
l = legend('show');
set(l, 'Interpreter', 'none');
saveas(fig, outputname);