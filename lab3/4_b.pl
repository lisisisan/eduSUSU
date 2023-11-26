/******************************************************************************
Даны четыре числа X, Y, Z и T. X меньше Y и меньше T; Y больше Z и больше T; Z больше X и меньше T. В каком порядке расположены эти числа?
*******************************************************************************/

гипотеза(_, [], []).
гипотеза(N, L, [N:V|T]) :- 
    select(V, L, LL), 
    NN is N+1,
    гипотеза(NN, LL, T).
 
больше(X, Y, H) :-
    member(NX:X, H),
    member(NY:Y, H),
    NX > NY.
 
меньше(X, Y, H) :-
    member(NX:X, H),
    member(NY:Y, H),
    NX < NY.
 
проверка(H) :-
    меньше(x, y, H),
    меньше(x, t, H),
    больше(y, z, H),
    больше(y, t, H),
    больше(z, x, H),
    меньше(z, t, H).
 
решение :-
    гипотеза(1, [x, y, z, t], H),
    проверка(H),
    write(H), nl,
    fail;
    write("----"), nl.