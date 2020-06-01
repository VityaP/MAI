selct(X,[X|Xs],Xs).
selct(X,[Y|Xs],[Y|Zs]):-selct(X,Xs,Zs).

perm(L,[X|P]):-selct(X,L,L1),perm(L1,P).
perm([],[]).