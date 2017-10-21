clear;
close all;

% Global variables
files(1,:) = '../solutions/cfg1_set1.txt.pareto';
files(2,:) = '../solutions/cfg1_set2.txt.pareto';
files(3,:) = '../solutions/cfg1_set3.txt.pareto';
%files(4,:) = '../solutions/4.txt.pareto';
%files(5,:) = '../solutions/5.txt.pareto';
%files(6,:) = '../solutions/6.txt.pareto';
%files(7,:) = '../solutions/7.txt.pareto';
%files(8,:) = '../solutions/8.txt.pareto';
outputname = './images/assn1d_cfg1_pareto';
colors = jet(size(files,1));
markers = {'o','*','x','s','d','v'};

fig = figure;
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(char(files(i,:)));
    runData = [];

    % Read in data
    j = 1;
    while 1
        line = fgetl(file);
        if ~ischar(line)
            break
        end
        if(~isempty(line))
            lineData = textscan(line,'%f,%f');
            lineData = cell2mat(lineData);
            obj1(j) = lineData(1);
            obj2(j) = lineData(2);
            j = j + 1;
        end
    end
    fclose(file);
    
    % Graph data
    average = mean(runData);
    plot(obj1,obj2,markers{i},'DisplayName',char(files(i,:)),'color',colors(i,:));
end

% Format graph
hold off;
title('Pareto Front');
xlabel('Objective 1');
ylabel('Objective 2');
l = legend('show');
set(l, 'Interpreter', 'none');

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0');
