clear;
%clf;

filenum = '048'; % file number for the data you want to read
infofile = strcat('INF', filenum, '.TXT');
datafile = strcat('LOG', filenum, '.BIN');

%% map from datatype to length in bytes
dataSizes.('float') = 4;
dataSizes.('ulong') = 4;
dataSizes.('int') = 4;
dataSizes.('int32') = 4;
dataSizes.('uint8') = 1;
dataSizes.('uint16') = 2;
dataSizes.('char') = 1;
dataSizes.('bool') = 1;

%% read from info file to get log file structure
fileID = fopen(infofile);
items = textscan(fileID,'%s','Delimiter',',','EndOfLine','\r\n');
fclose(fileID);
[ncols,~] = size(items{1});
ncols = ncols/2;
varNames = items{1}(1:ncols)';
varTypes = items{1}(ncols+1:end)';
varLengths = zeros(size(varTypes));
colLength = 256;
for i = 1:numel(varTypes)
    varLengths(i) = dataSizes.(varTypes{i});
end
R = cell(1,numel(varNames));

%% read column-by-column from datafile
fid = fopen(datafile,'rb');
for i=1:numel(varTypes)
    %# seek to the first field of the first record
    fseek(fid, sum(varLengths(1:i-1)), 'bof');
    
    %# % read column with specified format, skipping required number of bytes
    R{i} = fread(fid, Inf, ['*' varTypes{i}], colLength-varLengths(i));
    eval(strcat(varNames{i},'=','R{',num2str(i),'};'));
end
fclose(fid);

%% Process your data here
dataTable = table(R{:}, 'VariableNames',varNames);
%head(dataTable) % Printed


cropped_data = dataTable(30:end,:);
x = cropped_data.lon; % change according to head
y = cropped_data.lat; % change
%time = cropped_data.time;
angerror = cropped_data.yaw - cropped_data.yaw_des;
control = cropped_data.u;

figure(1)
gpsgraph = imread('gpsgraph.png');

C = size(gpsgraph);
P = ((x+117.712)*1200000) + 1500;
Q = (1400000*(34.1065-y)) + 700;

image(gpsgraph)
hold on;
plot(P, Q, '-s','MarkerSize',10,...
    'MarkerEdgeColor','red',...
    'MarkerFaceColor',[1 .6 .6]);
xlabel('X Axis');
ylabel('Y Axis');
title('Measured x, y Positions from Parsons Walk');
hold off;

figure(2)
plot(angerror);
title('Angle Error vs. Time');
xlabel('Sample number');
ylabel('Angle Error');
figure(3)
plot(control);
title('Control Effort vs Time');
xlabel('Sample number');
ylabel('Control Effort');