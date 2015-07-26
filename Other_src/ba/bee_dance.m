 function U=bee_dance(ngh,x1, x2)  % performs the local search step on (x1,x2) with  patch size radius of ngh
    U(:,1)=(x1-ngh)+(2*ngh.*rand(size(x1,1),1));
    U(:,2)=(x2-ngh)+(2*ngh.*rand(size(x2,1),1));
  end