clear;
close all;

set(0,'DefaultFigureVisible','off');

% Global variables
files = [];
files(1,:)  = 'cfg1_set1.txt              ';
files(2,:)  = 'cfg1_set2.txt              ';
files(3,:)  = 'cfg1_set3.txt              ';
files(4,:)  = 'cfg1_control_set1.txt      ';
files(5,:)  = 'cfg1_control_set2.txt      ';
files(6,:)  = 'cfg1_control_set3.txt      ';
files(7,:)  = 'cfg2_no_mutation_set1.txt  ';
files(8,:)  = 'cfg2_no_mutation_set2.txt  ';
files(9,:)  = 'cfg2_no_mutation_set3.txt  ';
files(10,:) = 'cfg2_half_mutation_set1.txt';
files(11,:) = 'cfg2_half_mutation_set2.txt';
files(12,:) = 'cfg2_half_mutation_set3.txt';
files(13,:) = 'cfg2_all_mutation_set1.txt ';
files(14,:) = 'cfg2_all_mutation_set2.txt ';
files(15,:) = 'cfg2_all_mutation_set3.txt ';
files(16,:) = 'cfg3_repair_set1.txt       ';
files(17,:) = 'cfg3_repair_set2.txt       ';
files(18,:) = 'cfg3_repair_set3.txt       ';
files(19,:) = 'cfg3_randreset_set1.txt    ';
files(20,:) = 'cfg3_randreset_set2.txt    ';
files(21,:) = 'cfg3_randreset_set3.txt    ';
files(22,:) = 'bonus3_set1.txt            ';
files(23,:) = 'bonus3_set2.txt            ';
files(24,:) = 'bonus3_set3.txt            ';

for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(['../logs/' char(files(i,:))]);
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
            avgFitness1(run,eval) = lineData(2);
            bestFitness1(run,eval) = lineData(3);
            avgFitness2(run,eval) = lineData(4);
            bestFitness2(run,eval) = lineData(5);
            eval = eval + 1;
        end
    end
    fclose(file);
    
    % Calculate averages
    for j=1:eval-1
        avg_avgFitness1(j) = mean(avgFitness1(:,j));
        avg_bestFitness1(j) = mean(bestFitness1(:,j));
        avg_avgFitness2(j) = mean(avgFitness2(:,j));
        avg_bestFitness2(j) = mean(bestFitness2(:,j));
    end

    % Make plots
    fig1 = figure;
    boxplot([avg_bestFitness1.',avg_avgFitness1.'],'Labels',{'Best Fitness','Average Fitness'})
    title({'Distribution of Fitness Values Across Evaluations','Objective 1',['Logfile: ' char(files(i,:))]},'Interpreter','none');
    ylabel('Fitness');
    
    fig2 = figure;
    boxplot([avg_bestFitness2.',avg_avgFitness2.'],'Labels',{'Best Fitness','Average Fitness'})
    title({'Distribution of Fitness Values Across Evaluations','Objective 2',['Logfile: ' char(files(i,:))]},'Interpreter','none');
    ylabel('Fitness');
    
    % Save image file2
    for j=1:size(files,2)
        if(char(files(i,j)) == '.')
            break
        end
        filename = files(i,1:j);
    end
    saveas(fig1, ['images\assn1d_boxplot_' filename '_obj1.png']);
    saveas(fig2, ['images\assn1d_boxplot_' filename '_obj2.png']);
    
end