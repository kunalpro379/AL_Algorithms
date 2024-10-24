% Base case: Move a single disk from Source to Destination
move(1, Source, Destination) :-
    write('Move disk from '), write(Source), write(' to '), write(Destination), nl.

% Recursive case: Move N disks from Source to Helper, then to Destination, and finally from Helper to Destination
move(N, Source, Destination) :-
    N > 1,
    Helper is 6 - Source - Destination,  % Helper peg is determined using the formula (1 + 2 + 3) - Source - Destination
    move(N-1, Source, Helper),           % Move N-1 disks from Source to Helper
    move(1, Source, Destination),        % Move the last disk from Source to Destination
    move(N-1, Helper, Destination).      % Move N-1 disks from Helper to Destination

% To solve the Tower of Hanoi for N disks
tower_of_hanoi(N) :-
    move(N, 1, 3).  % Assuming pegs are numbered as 1 (Source), 2 (Helper), and 3 (Destination)





?- tower_of_hanoi(3).
