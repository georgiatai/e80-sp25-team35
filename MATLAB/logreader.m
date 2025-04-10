% logreader.m
% Use this script to read data from your micro SD card

clear;
%clf;

filenum = '031'; % file number for the data you want to read
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

dataTable1 = table(R{:}, 'VariableNames',varNames);
mx = dataTable1.magX;
my = dataTable1.magY;

% logreader.m
% Use this script to read data from your micro SD card
%clf;

filenum = '034'; % file number for the data you want to read
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

dataTable2 = table(R{:}, 'VariableNames',varNames);
mx_cal = dataTable2.magX;
my_cal = dataTable2.magY;

%% Process your data here

sample_freq = 10.1; % Hz
sample_rate = 1/sample_freq;
time = 0:sample_rate:(sample_rate*(length(mx)-1));
time2 = 0:sample_rate:(sample_rate*(length(mx_cal)-1));

% Plotting mx and my
figure(1)
scatter(mx, my, "red");
hold on
scatter(mx_cal, my_cal, "blue");
xlabel('X Magnetic Flux [mG]');
ylabel('Y Magnetic Flux [mG]');
title('Magnetometer Amgentic Induction');
legend('Uncalibrated', 'Calibrated');
hold off;

% Calculating heading
% LSB_val = 0.48828125;

% not sure if this is right or necessary, maybe ours is already processed
% mx = mx.*LSB_val;
% my = my.*LSB_val;
% mx_cal = mx_cal.*LSB_val;
% my_cal = my_cal.*LSB_val;

D = zeros(length(mx));
DCal = zeros(length(mx_cal));

for i = 1:length(mx)
    if (mx(i) == 0)
         if (my(i) < 0)
             D(i) = 90;
         else
             D(i) = 0;
         end
    else
        D(i) = atan(my(i)/mx(i));
    end
end

for i = 1:length(mx_cal)
    if (mx_cal(i) == 0)
         if (my_cal(i) < 0)
             DCal(i) = 90;
         else
             DCal(i) = 0;
         end
    else
        DCal(i) = atan(my_cal(i)/mx_cal(i));
    end
end

% Plot Compass Heading
figure(2)
hold on;
plot(time, D, 'r');
plot(time2, DCal, 'b');
title('Compass Heading vs. Time')
legend('Uncalibrated', 'Calibrated');
xlabel('time [s]');
ylabel('Heading [radians]');
hold off;
