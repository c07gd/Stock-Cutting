clear;
close all;

set(0,'DefaultFigureVisible','off');

% Global variables
files = [];
files(1,:) = '../logs/cfg1_set1.txt';
files(2,:) = '../logs/cfg1_set2.txt';
files(3,:) = '../logs/cfg1_set3.txt';

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
            lineData = textscan(line,'%f %f %f %f %f');
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
    filename = [];
    for j=9:size(files,2)
        if(char(files(i,j)) == '.')
            break
        end
        filename(j-8) = files(i,j);
    end
    saveas(fig, ['images\assn1c_boxplot_' filename '.png']);
    
end