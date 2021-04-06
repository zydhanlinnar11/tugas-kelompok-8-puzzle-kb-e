#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include "Puzzle.h"

bool operator > (const Puzzle &a, const Puzzle &b) {
    return a.heuristicValue > b.heuristicValue;
}

void printPuzzleMovements(Puzzle initialPuzzle, string moves) {
    cout<<"Puzzle awal:\n";
    cout<<initialPuzzle<<'\n';

    cout<<"Langkah-langkah:\n";
    for(auto i: moves) {
        initialPuzzle = initialPuzzle.movePuzzle(i);
        cout<<initialPuzzle<<'\n';
    }
}

void greedyBFS(string board, string goal) {
    Puzzle puzzle = Puzzle(board, true, Puzzle::searchZeroPos(board), "", goal);
    Puzzle solvedPuzzle = puzzle;

    set<string> visitedBoard;
    priority_queue<Puzzle, vector<Puzzle>, greater<Puzzle>> greedyPriorityQueue;
    greedyPriorityQueue.push(puzzle);
    int nodeExplored = 0, closedList = 0;
    while(!greedyPriorityQueue.empty()) {
        nodeExplored++;
        Puzzle curr = greedyPriorityQueue.top(); greedyPriorityQueue.pop();
        visitedBoard.insert(curr.getBoard());

        if(curr.heuristicValue == 0) {
            solvedPuzzle = curr;
            break;
        }

        for(auto movement: Puzzle::movements) {
            if(!curr.isLegalMove(movement))
                continue;

            Puzzle movedPuzzle = curr.movePuzzle(movement);
        
            if(visitedBoard.find(movedPuzzle.getBoard()) == visitedBoard.end())
                greedyPriorityQueue.push(movedPuzzle), closedList++;
        }
    }

    if(solvedPuzzle.moves == "") {
        cout<<"Unsolvable puzzle\n";
        return;
    }

    printPuzzleMovements(puzzle, solvedPuzzle.moves);
    cout<<"Open lists: "<<nodeExplored<<'\n';
    cout<<"Closed lists: "<<closedList<<'\n';
    cout<<"Steps required: "<<solvedPuzzle.moves.length()<<'\n';
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
    
    greedyBFS(board, goal);

    // Uncomment code dibawah jika ingin custom input melalui puzzle.txt
    inp.close();
    // Uncomment code diatas jika ingin custom input melalui puzzle.txt

    before = clock() - before;
    cout<<(double)before<<" milidetik.\n";
    return 0;
}