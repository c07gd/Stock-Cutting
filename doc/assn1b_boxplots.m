clear;
close all;

% Global variables
files = [];
files (1,:) = '../logs/log_instance1.txt';
files (2,:) = '../logs/log_instance2.txt';
files (3,:) = '../logs/log_instance3.txt';

for i=1:size(files,1)
    
    % Variables for this run
    avgFitness = [];
    avg_avgFitness = [];
    bestFitness = [];
    avg_bestFitness = [];
    file = fopen(char(files(i,:)));
    run = 1;
    eval = 1;

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
            continue
        end
        if(~isempty(line))
            lineData = textscan(line,'%f %f %f');
            lineData = cell2mat(lineData);
            avgFitness(run,eval) = lineData(2);
            bestFitness(run,eval) = lineData(3);
            eval = eval + 1;
        end
    end
    fclose(file);
    
    % Calculate averages
    for j=1:eval-1
        avg_avgFitness(j) = mean(avgFitness(:,j));
        avg_bestFitness(j) = mean(bestFitness(:,j));
    end

    % Make plots
    fig = figure;
    boxplot([avg_bestFitness.',avg_avgFitness.'],'Labels',{'Best Fitness','Average Fitness'})
    title({'Distribution of Fitness Values Across Evaluations';['Logfile: ' char(files(i,:))]},'Interpreter','none');
    ylabel('Fitness');
    saveas(fig, ['images\graph_' num2str(i) '.png']);
    
end