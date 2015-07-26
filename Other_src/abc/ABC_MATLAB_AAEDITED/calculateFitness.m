function fFitness=calculateFitness(fObjV)  % the argument is an array of double with solution values

   % zero() : Creates an array of zeros of the specified size. 
   % if fObjV has n variables, create an array with n variables, set to zero
fFitness=zeros(size(fObjV)); 

% find() : Returns a vector that contains the indicies of all non-zero elements in an array.
% here all indicies are returned, that have fObjV value greater zero
ind=find(fObjV>=0);

fFitness(ind)=1./(fObjV(ind)+1);

ind=find(fObjV<0);

fFitness(ind)=1+abs(fObjV(ind));

