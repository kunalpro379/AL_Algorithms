% Facts
parent(john, mary).         % John is a parent of Mary
parent(john, james).        % John is a parent of James
parent(susan, mary).        % Susan is a parent of Mary
parent(susan, james).       % Susan is a parent of James
parent(mary, emily).        % Mary is a parent of Emily
parent(mary, david).        % Mary is a parent of David
parent(james, sofia).       % James is a parent of Sofia

% Defining relationships
male(john).
male(james).
male(david).

female(susan).
female(mary).
female(emily).
female(sofia).

% Rules
sibling(X, Y) :- 
    parent(P, X), 
    parent(P, Y), 
    X \= Y.

grandparent(GP, GC) :- 
    parent(GP, P), 
    parent(P, GC).

aunt_or_uncle(AU, N) :- 
    sibling(AU, P), 
    parent(P, N).

cousin(X, Y) :- 
    parent(P1, X), 
    parent(P2, Y), 
    sibling(P1, P2).




?- sibling(mary, james).
?- grandparent(john, emily).
?- aunt_or_uncle(AU, emily).
?- cousin(emily, sofia).
?- parent(P, emily).
