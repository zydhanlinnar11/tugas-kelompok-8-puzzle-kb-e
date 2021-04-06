#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include "Puzzle.h"

void printPuzzleMovements(Puzzle initialPuzzle, string moves) {
    cout<<"Puzzle awal:\n";
    cout<<initialPuzzle<<'\n';

    cout<<"Langkah-langkah:\n";
    for(auto i: moves) {
        initialPuzzle = initialPuzzle.movePuzzle(i);
        cout<<initialPuzzle<<'\n';
    }
}

string reverseMoves(string moves) {
    reverse(moves.begin(), moves.end());
    for(auto &i: moves)
        i = Puzzle::reverseDirections[i];
    return moves;
}

void bidirectional_search(string board, string goal) {
    Puzzle puzzleFromStart = Puzzle(board, false, Puzzle::searchZeroPos(board), "", goal);
    Puzzle puzzleFromGoal = Puzzle(goal, false, Puzzle::searchZeroPos(goal), "", board);

    queue<Puzzle> queueFromStart, queueFromGoal;
    set<string> boardFromStart, boardFromGoal;
    map<string, string> movesFromStart, movesFromGoal;
    queueFromStart.push(puzzleFromStart);
    queueFromGoal.push(puzzleFromGoal);
    int nodeExplored = 0, closedList = 0;

    string shortestMoves = "";
    while(!queueFromStart.empty() || !queueFromGoal.empty()) {
        if(!queueFromStart.empty()) {
            nodeExplored++;
            Puzzle currFromStart = queueFromStart.front(); queueFromStart.pop();
            boardFromStart.insert(currFromStart.getBoard());
            movesFromStart[currFromStart.getBoard()] = currFromStart.moves;
            
            for(auto movement: Puzzle::movements) {
                if(!currFromStart.isLegalMove(movement))
                    continue;
                Puzzle movedPuzzle = currFromStart.movePuzzle(movement);
                if(boardFromStart.find(movedPuzzle.getBoard()) == boardFromStart.end())
                    queueFromStart.push(movedPuzzle), closedList++;
            }
        }

        if(!queueFromGoal.empty()) {
            nodeExplored++;
            Puzzle currFromGoal = queueFromGoal.front(); queueFromGoal.pop();
            boardFromGoal.insert(currFromGoal.getBoard());
            movesFromGoal[currFromGoal.getBoard()] = currFromGoal.moves;
            
            for(auto movement: Puzzle::movements) {
                if(!currFromGoal.isLegalMove(movement))
                    continue;
                Puzzle movedPuzzle = currFromGoal.movePuzzle(movement);
                if(boardFromGoal.find(movedPuzzle.getBoard()) == boardFromGoal.end())
                    queueFromGoal.push(movedPuzzle), closedList++;
            }
        }

        vector<string> intersections;
        set_intersection(boardFromStart.begin(), boardFromStart.end(),
                        boardFromGoal.begin(), boardFromGoal.end(),
                        back_inserter(intersections));
                        
        if(!intersections.empty()) {
            string firstIntersection = intersections[0];
            shortestMoves = movesFromStart[firstIntersection] + reverseMoves(movesFromGoal[firstIntersection]);
            for(auto i: intersections) {
                string reversedMovesFromGoal = reverseMoves(movesFromGoal[i]);
                if(movesFromStart[i].length() + reversedMovesFromGoal.length() < shortestMoves.length())
                    shortestMoves = movesFromStart[i] + reversedMovesFromGoal;
            }
            break;
        }
    }

    if(shortestMoves == "") {
        cout<<"Unsolvable puzzle\n";
        return;
    }

    printPuzzleMovements(puzzleFromStart, shortestMoves);
    cout<<"Open lists: "<<nodeExplored<<'\n';
    cout<<"Closed lists: "<<closedList<<'\n';
    cout<<"Steps required: "<<shortestMoves.length()<<'\n';
}

#include <time.h>
#include <cstdlib>
int main() {
    clock_t before = clock();
    // Sample testcase
    string board = "724506831";
    string goal = "012345678";

    // Uncomment code dibawah jika ingin custom input melalui puzzle.txt
    auto inp = ifstream("puzzle.txt");

    inp>>board;
    inp>>goal;
    // Uncomment code diatas jika ingin custom input melalui puzzle.txt
    
    bidirectional_search(board, goal);

    // Uncomment code dibawah jika ingin custom input melalui puzzle.txt
    inp.close();
    // Uncomment code diatas jika ingin custom input melalui puzzle.txt

    before = clock() - before;
    cout<<(double)before<<" milidetik.\n";
    return 0;
}