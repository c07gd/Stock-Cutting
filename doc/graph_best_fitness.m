clear;
close all;

% Configuration
cfg = 2;
switch cfg
    case 1
        files(1,:) = 'cfg1_set1.txt        ';
        files(2,:) = 'cfg1_set2.txt        ';
        files(3,:) = 'cfg1_set3.txt        ';
        files(4,:) = 'cfg1_control_set1.txt';
        files(5,:) = 'cfg1_control_set2.txt';
        files(6,:) = 'cfg1_control_set3.txt';
        outputname = 'assn1d_cfg1_bestfitness.png';
    case 2
        files(1,:) = 'cfg2_no_mutation_set1.txt  ';
        files(2,:) = 'cfg2_no_mutation_set2.txt  ';
        files(3,:) = 'cfg2_no_mutation_set3.txt  ';
        files(4,:) = 'cfg2_half_mutation_set1.txt';
        files(5,:) = 'cfg2_half_mutation_set2.txt';
        files(6,:) = 'cfg2_half_mutation_set3.txt';
        files(7,:) = 'cfg2_all_mutation_set1.txt ';
        files(8,:) = 'cfg2_all_mutation_set2.txt ';
        files(9,:) = 'cfg2_all_mutation_set3.txt ';
        outputname = 'assn1d_cfg2_bestfitness.png';
    case 3
        files(1,:) = 'cfg3_repair_set1.txt   ';
        files(2,:) = 'cfg3_repair_set2.txt   ';
        files(3,:) = 'cfg3_repair_set3.txt   ';
        files(4,:) = 'cfg3_randreset_set1.txt';
        files(5,:) = 'cfg3_randreset_set2.txt';
        files(6,:) = 'cfg3_randreset_set3.txt';
        outputname = 'assn1d_cfg3_bestfitness.png';
    otherwise
        exit;
end

% Variables
colors = jet(size(files,1));

fig = figure();
subplot(2,1,1);
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(['../logs/' char(files(i,:))]);
    run = 0;
    eval = 1;
    best1 = 0;
    best2 = 0;

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
            runData1(run) = best1;
            runData2(run) = best2;
            best1 = 0;
            best2 = 0;
            continue
        end
        if(~isempty(line))
            lineData = textscan(line,'%f %f %f %f %f');
            lineData = cell2mat(lineData);
            if(lineData(3) > best1)
                best1 = lineData(3);
            end
            if(lineData(5) > best2)
                best2 = lineData(5);
            end
            eval = eval + 1;
        end
    end
    fclose(file);
    
    % Graph data
    p1 = plot(1:1:length(runData1),runData1,'DisplayName',char(files(i,:)),'color',colors(i,:));
    p2 = plot(1:1:length(runData2),runData2,'DisplayName',char(files(i,:)),'color',colors(i,:));
    hasbehavior(p2,'legend',false);
    averages(i,1) = mean(runData1);
    averages(i,2) = mean(runData2);
    best(i,1) = max(runData1);
    best(i,2) = max(runData2);
end

% Format graph
hold off;
title('Best Fitnesses');
xlabel('Run');
ylabel('Fitness');

% Build and format table
for i=1:size(files,1)
    colorstring(i,:) = ['<html><span style="color: rgb(' sprintf('%03d',floor(255*colors(i,1))) ',' sprintf('%03d',floor(255*colors(i,2))) ',' sprintf('%03d',floor(255*colors(i,3))) ');">---------------</span></html>"'];
end
tabledata = table(cellstr(colorstring),cellstr(files),averages(:,1),best(:,1),averages(:,2),best(:,2));
tabledata = table2cell(tabledata);
t = uitable();
t.Data = tabledata;
t.Units = 'normalized';
t.Position = [0,-.01,1,.5];
t.ColumnName = {'Color','File','Average Best|Obj 1','Overall Best|Obj 1','Average Best|Obj 2','Overall Best|Obj 2'};
t.ColumnWidth = {50,170,78,78,78,78};

% Save image
fig.PaperPositionMode = 'auto';
print(['./images/' outputname],'-dpng','-r0');

% Show figure
figure(fig);
