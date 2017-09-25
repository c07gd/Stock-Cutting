clear;

filename = '../logs/assn1a_log_instance1.txt';

rawData = importdata(filename);
evals = rawData.data(:,1);
fitness = rawData.data(:,2);

[max, maxIdx] = max(fitness);

minIdx = maxIdx;
while(fitness(minIdx) ~= 0)
    minIdx = minIdx - 1;
end

x = [evals(minIdx:maxIdx); 1000];
y = [fitness(minIdx:maxIdx); fitness(maxIdx)];

fig = figure;
%set(fig, 'Visible', 'off');

stairs(x,y);
title('Evals vs. Fitness for Best Solution');
xlabel('Evaluation');
ylabel('Fitness');
axis([0 1000 0 fitness(maxIdx)+1]);

saveas(fig, ['images\graph_test.png']);
