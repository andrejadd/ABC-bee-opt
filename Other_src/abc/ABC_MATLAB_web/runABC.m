%%%%%ARTIFICIAL BEE COLONY ALGORITHM%%%%

%Artificial Bee Colony Algorithm was developed by Dervis Karaboga in 2005 
%by simulating the foraging behaviour of bees.

%Copyright  2008 Erciyes University, Intelligent Systems Research Group, The Dept. of Computer Engineering

%Contact:
%Dervis Karaboga (karaboga@erciyes.edu.tr )
%Bahriye Basturk Akay (bahriye@erciyes.edu.tr)


clear all
close all
clc



% Set ABC Control Parameters
ABCOpts = struct( 'ColonySize',  10, ...   % Number of Employed Bees+ Number of Onlooker Bees 
    'MaxCycles', 2000,...   % Maximum cycle number in order to terminate the algorithm
    'ErrGoal',     1e-20, ...  % Error goal in order to terminate the algorithm (not used in the code in current version)
    'Dim',       10 , ... % Number of parameters of the objective function   
    'Limit',   100, ... % Control paramter in order to abandone the food source 
    'lb',  -3, ... % Lower bound of the parameters to be optimized
    'ub',  3, ... %Upper bound of the parameters to be optimized
    'ObjFun' , 'rosenbrock', ... %Write the name of the objective function you want to minimize
    'RunTime',3); % Number of the runs 



GlobalMins=zeros(ABCOpts.RunTime,ABCOpts.MaxCycles);

for r=1:ABCOpts.RunTime
    
% Initialise population
Range = repmat((ABCOpts.ub-ABCOpts.lb),[ABCOpts.ColonySize ABCOpts.Dim]);
Lower = repmat(ABCOpts.lb, [ABCOpts.ColonySize ABCOpts.Dim]);
Colony = rand(ABCOpts.ColonySize,ABCOpts.Dim) .* Range + Lower;

Employed=Colony(1:(ABCOpts.ColonySize/2),:);


%evaluate and calculate fitness
ObjEmp=feval(ABCOpts.ObjFun,Employed);
FitEmp=calculateFitness(ObjEmp);

%set initial values of Bas, what is Bas
Bas=zeros(1,(ABCOpts.ColonySize/2));

% extract lowest solution value from ObjEmp
GlobalMin=ObjEmp(find(ObjEmp==min(ObjEmp),end));
GlobalParams=Employed(find(ObjEmp==min(ObjEmp),end),:); % these are the corresponding parameters

Cycle=1;
while ((Cycle <= ABCOpts.MaxCycles)),
    
    %%%%% Employed phase
    Employed2=Employed;
    for i=1:ABCOpts.ColonySize/2
        Param2Change=fix(rand*ABCOpts.Dim)+1;
        neighbour=fix(rand*(ABCOpts.ColonySize/2))+1;
            while(neighbour==i)
                neighbour=fix(rand*(ABCOpts.ColonySize/2))+1;
            end;
        Employed2(i,Param2Change)=Employed(i,Param2Change)+(Employed(i,Param2Change)-Employed(neighbour,Param2Change))*(rand-0.5)*2;
         if (Employed2(i,Param2Change)<ABCOpts.lb) % if outside lower bound, set to lower bound
             Employed2(i,Param2Change)=ABCOpts.lb;
         end;
        if (Employed2(i,Param2Change)>ABCOpts.ub) % if outside upper bound, set to upper bound
            Employed2(i,Param2Change)=ABCOpts.ub;
        end;
        
end;   

% evaluate the modified solutions
ObjEmp2=feval(ABCOpts.ObjFun,Employed2);

% and calculate fittness
FitEmp2=calculateFitness(ObjEmp2);

% update the EB with the improved solutions
 [Employed ObjEmp FitEmp Bas]=GreedySelection(Employed,Employed2,ObjEmp,ObjEmp2,FitEmp,FitEmp2,Bas,ABCOpts);
    
    %Normalize, is equal to the selection prob. equ
    NormFit=FitEmp/sum(FitEmp);
    
    %%% Onlooker phase  
Employed2=Employed;
i=1;
t=0;
while(t<ABCOpts.ColonySize/2)
    if(rand<NormFit(i)) % RWS, i will be the index of selected EB
        t=t+1;
        Param2Change=fix(rand*ABCOpts.Dim)+1;
        neighbour=fix(rand*(ABCOpts.ColonySize/2))+1;
            while(neighbour==i)
                neighbour=fix(rand*(ABCOpts.ColonySize/2))+1;
            end;
         Employed2(i,:)=Employed(i,:);
         Employed2(i,Param2Change)=Employed(i,Param2Change)+(Employed(i,Param2Change)-Employed(neighbour,Param2Change))*(rand-0.5)*2;
         if (Employed2(i,Param2Change)<ABCOpts.lb)
             Employed2(i,Param2Change)=ABCOpts.lb;
         end;
        if (Employed2(i,Param2Change)>ABCOpts.ub)
            Employed2(i,Param2Change)=ABCOpts.ub;
         end;
    ObjEmp2=feval(ABCOpts.ObjFun,Employed2);
    FitEmp2=calculateFitness(ObjEmp2);
    [Employed ObjEmp FitEmp Bas]=GreedySelection(Employed,Employed2,ObjEmp,ObjEmp2,FitEmp,FitEmp2,Bas,ABCOpts,i);
   
   end;
    
    i=i+1;
    if (i==(ABCOpts.ColonySize/2)+1) 
        i=1;
    end;   
end;
    
    
    %%%Memorize Best
 CycleBestIndex=find(FitEmp==max(FitEmp));
 CycleBestIndex=CycleBestIndex(end);
 CycleBestParams=Employed(CycleBestIndex,:);
 CycleMin=ObjEmp(CycleBestIndex);
 
 if CycleMin<GlobalMin  % save the best solution as GlobalMin
       GlobalMin=CycleMin;
       GlobalParams=CycleBestParams;
 end
 
 GlobalMins(r,Cycle)=GlobalMin;
 
 %% Scout phase
 ind=find(Bas==max(Bas));  % return array index of the solutions with max abandon counter

% Always only one can become scout, because ind(end) returns last entry from ind
 ind=ind(end);
  
if (Bas(ind)>ABCOpts.Limit) % check if limit surpassed
Bas(ind)=0;    % reset abandon counter
Employed(ind,:)=(ABCOpts.ub-ABCOpts.lb)*(0.5-rand(1,ABCOpts.Dim))*2;%+ABCOpts.lb;
%message=strcat('burada',num2str(ind))
end;

% recalculate all values and fitness, this should not be here, because function evaluations 
% is increased. only the abandoned and replaced solution need recalc
ObjEmp=feval(ABCOpts.ObjFun,Employed);
FitEmp=calculateFitness(ObjEmp);
    


    fprintf('Cycle=%d ObjVal=%g\n',Cycle,GlobalMin);
    
    Cycle=Cycle+1;

end % End of ABC

end; %end of runs

semilogy(mean(GlobalMins))
title('Mean of Best function values');
xlabel('cycles');
ylabel('error');
fprintf('Mean =%g Std=%g\n',mean(GlobalMins(:,end)),std(GlobalMins(:,end)));
  
