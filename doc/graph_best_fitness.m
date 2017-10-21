clear;
close all;

% Global variables
files(1,:) = '../logs/cfg1_set1.txt';
files(2,:) = '../logs/cfg1_set2.txt';
files(3,:) = '../logs/cfg1_set3.txt';
%files(4,:) = '../logs/4.txt';
%files(5,:) = '../logs/5.txt';
%files(6,:) = '../logs/6.txt';
%files(7,:) = '../logs/7.txt';
%files(8,:) = '../logs/8.txt';
outputname = './images/assn1d_cfg1_bestfitness';
colors = jet(size(files,1));

fig = figure();
subplot(2,1,1);
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(char(files(i,:)));
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
t.Position = [0,-.04,1,.5];
t.ColumnName = {'Color','File','Average Best|Obj 1','Overall Best|Obj 1','Average Best|Obj 2','Overall Best|Obj 2'};
t.ColumnWidth = {50,140,80,80,80,80};

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0');
