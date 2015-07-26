
% Standard Einstellung , Sphere Fct. , Achtung: Limit muss zusammen mit ColonySize geändert werden
% ABCOpts = struct( 'ColonySize',  30, ...   % Number of Employed Bees+ Number of Onlooker Bees 
    'MaxCycles', 20000,...   % Maximum cycle number in order to terminate the algorithm
    'ErrGoal',   0.01, ...  %^1e-20, ...  % Error goal in order to terminate the algorithm (not used in the code in current version)
    'Dim',       30, ... % Number of parameters of the objective function   
    'Limit',   1500, ... % Control paramter in order to abandone the food source 
    'lb',  -100, ... % Lower bound of the parameters to be optimized
    'ub',  100, ... %Upper bound of the parameters to be optimized
    'ObjFun' , 'Sphere', ... %Write the name of the objective function you want to minimize
    'RunTime',10);      % Number of the runs 
