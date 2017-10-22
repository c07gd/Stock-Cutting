clear;
close all;

% Configuration
cfg = 4;
switch cfg
    case 1
        files(1,:) = 'cfg1_set1.txt.pareto        ';
        files(2,:) = 'cfg1_set2.txt.pareto        ';
        files(3,:) = 'cfg1_set3.txt.pareto        ';
        files(4,:) = 'cfg1_control_set1.txt.pareto';
        files(5,:) = 'cfg1_control_set2.txt.pareto';
        files(6,:) = 'cfg1_control_set3.txt.pareto';
        outputname = 'assn1d_cfg1_pareto.png';
    case 2
        files(1,:) = 'cfg2_no_mutation_set1.txt.pareto  ';
        files(2,:) = 'cfg2_no_mutation_set2.txt.pareto  ';
        files(3,:) = 'cfg2_no_mutation_set3.txt.pareto  ';
        files(1,:) = 'cfg2_no_mutation_set1.txt.pareto  ';
        files(2,:) = 'cfg2_no_mutation_set2.txt.pareto  ';
        files(3,:) = 'cfg2_no_mutation_set3.txt.pareto  ';
        files(4,:) = 'cfg2_half_mutation_set1.txt.pareto';
        files(5,:) = 'cfg2_half_mutation_set2.txt.pareto';
        files(6,:) = 'cfg2_half_mutation_set3.txt.pareto';
        files(7,:) = 'cfg2_all_mutation_set1.txt.pareto ';
        files(8,:) = 'cfg2_all_mutation_set2.txt.pareto ';
        files(9,:) = 'cfg2_all_mutation_set3.txt.pareto ';
        outputname = 'assn1d_cfg2_pareto.png';
    case 3
        files(1,:) = 'cfg3_repair_set1.txt.pareto   ';
        files(2,:) = 'cfg3_repair_set2.txt.pareto   ';
        files(3,:) = 'cfg3_repair_set3.txt.pareto   ';
        files(4,:) = 'cfg3_randreset_set1.txt.pareto';
        files(5,:) = 'cfg3_randreset_set2.txt.pareto';
        files(6,:) = 'cfg3_randreset_set3.txt.pareto';
        outputname = 'assn1d_cfg3_pareto.png';
    case 4
        files(1,:) = 'bonus3_set1.txt.pareto';
        files(2,:) = 'bonus3_set2.txt.pareto';
        files(3,:) = 'bonus3_set3.txt.pareto';
        outputname = 'assn1d_bonus3_pareto.png';
    otherwise
        exit;
end

% Variables
colors = jet(size(files,1));
markers = {'o','s','d','p','h','v','^','>','<','*','x','+'};

fig = figure;
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(['../solutions/' char(files(i,:))]);
    runData = [];

    % Read in data
    j = 1;
    obj1 = [];
    obj2 = [];
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
    p = plot(obj1,obj2,markers{i});
    p.MarkerSize = 10;
    p.DisplayName = char(files(i,:));
    p.Color = colors(i,:);
    p.MarkerFaceColor = colors(i,:);
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
print(['./images/' outputname],'-dpng','-r0');

% Show figure
figure(fig);
