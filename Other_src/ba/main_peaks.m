% this code makes use of matlab function peaks, which evals a coordinates value in 3d
% peaks() is not included with freemat, so alot should be rewritten here (but not done) 


function main_peaks
clc; close all; clear all;


n= 30;      % number of scout bees (e.g. 40-100)
itr=15;     % number of iterations (e.g. 1000-5000)
m=20;       % number of best selected patches (e.g. 10-50)
e=10;       % number of elite selected patches (e.g. 10-50)
n1=15;      % number of recruited bees around best selected patches (e.g. 10-50)
n2=30;      % number of recruited bees around elite selected patches (e.g. 10-50)
ngh=0.0234; % Patch radius for neighbourhood search
x_max=3;
y_max=3;
x_min=-3;
y_min=-3;
Grid_step=0.05;


% Scoutbees: random search (only once)
U=X_random(n,x_max, y_max, x_min, y_min);

% ax(1)=plot3(U(:,1),U(:,2),peaks(U(:,1),U(:,2)),'k.');

Par_Q=sortrows([U(:,1), U(:,2), peaks(U(:,1),U(:,2))],-3);

clear U; l=0;



for k=1:itr         % iterations
    disp(sprintf('Interation Number: %02.0f',k));
    % ______________________________________________________________________
    for j=1:e       % number of elite selected patches
        for i=1:n2  % number of bees around elite patches
            U=bee_dance(ngh, Par_Q(j,1), Par_Q(j,2));
            if peaks(U(1),U(2))> Par_Q(j,3)
                Par_Q(j,:)=[U(1), U(2), peaks(U(1),U(2))];
                ax(1)=plot3(U(1), U(2), peaks(U(1), U(2)),'r.');
            end
            l=l+1;
        end
    end
    %  _______________________________________________________________________
    for j=e+1:m       % number of best selected patches
        for i=1 : n1   % number of bees around best patches
            U=bee_dance(ngh,Par_Q(j,1),Par_Q(j,2));
            if peaks(U(1),U(2))> Par_Q(j,3)
                Par_Q(j,:)=[U(1), U(2), peaks(U(1), U(2))];
                ax(1)=plot3 (U(1), U(2), peaks(U(1), U(2)), 'b.');
            end
            l=l+1;
        end
    end
    % _______________________________________________________________________
    for i=m+1:n
        U=X_random(2,x_max, y_max, x_min, y_min);
        Par_Q(i,:)=[U(1), U(2), peaks(U(1),U(2))];
        ax(1)=plot3(U(1), U(2), peaks(U(1), U(2)), 'k.');
        l=l+1;
     end
     
    % _______________________________________________________________________
    Par_Q=sortrows(Par_Q,-3);
    Best(k,:)=Par_Q(1:15,3)';
    
    end
    
    