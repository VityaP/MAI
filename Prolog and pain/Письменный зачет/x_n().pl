x_n(X,0,R,R).
x_n(X,N,P,R):- N1 is N-1,P1 is P*X,x_n(X,N1,P1,R).
x_n(X,N,R):-x_n(X,N,1,R),!.