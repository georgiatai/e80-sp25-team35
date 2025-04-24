% Magnetometer Plotting
%
% Team 35, Spring 2025
%
% Inputs: mx, my, mx_cal, my_cal
% Units: mG (as given by Mag_Calibration.m)

function magplot(mx, my, mx_cal, my_cal)

sample_freq = 10.1; % Hz
sample_rate = 1/sample_freq;
time = 0:sample_rate:(sample_rate*(length(mx)-1));

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
% xGauss = mx.*LSB_val;
% yGauss = my.*LSB_val;
% xGaussCal = mx_cal.*LSB_val;
% yGaussCal = my_cal.*LSB_val;

D = zeros(length(xGauss));
DCal = zeros(length(xGaussCal));

for i = 1:length(xGauss)
    if (xGauss(i) == 0)
         if (yGauss(i) < 0)
             D(i) = 90;
         else
             D(i) = 0;
         end
    else
        D(i) = arctan(yGauss(i)/xGauss(i));
    end
end

for i = 1:length(xGaussCal)
    if (xGaussCal(i) == 0)
         if (yGaussCal(i) < 0)
             DCal(i) = 90;
         else
             DCal(i) = 0;
         end
    else
        DCal(i) = arctan(yGaussCal(i)/xGaussCal(i));
    end
end

% Plot Compass Heading
figure(2)
plot(time, D, '-.', 'red');
hold on;
plot(time, DCal, '-.', 'blue');
title('Compass Heading vs. Time')
legend('Uncalibrated', 'Calibrated');
xlabel('time [s]');
ylabel('Heading [radians]');
hold off;

end