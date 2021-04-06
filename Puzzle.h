#include <iostream>
#include <map>
using namespace std;
typedef pair<int, int> pii;

class Puzzle {
    private:
        string board;
        pii zeroPos;
        string goal;
        map<char, pair<int, int>> goalElementsPos;
        bool heuristicNeeded;

        static int _2dTo1dCoordinate(int y, int x) { return y * 3 + x; }

        static pii _1dTo2dCoordinate(int coordinate) { return make_pair(coordinate / 3, coordinate % 3); }

        void calculateHeuristic(string board) {
            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                    this->goalElementsPos[goal[_2dTo1dCoordinate(i, j)]] = make_pair(i, j);

            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++) {
                    pii _2d_coordinate = this->goalElementsPos[board[_2dTo1dCoordinate(i, j)]];
                    if(board[_2dTo1dCoordinate(i, j)] != '0')
                        this->heuristicValue += abs(i - _2d_coordinate.first) + abs(j - _2d_coordinate.second);
                }
        }

    public:
        static map<char, pii> directions;
        static map<char, char> reverseDirections;
        static vector<char> movements;
        int heuristicValue;
        string moves;

        Puzzle(string board, bool heuristicNeeded, pii zeroPos, string moves, string goal) {
            this->board = board;
            this->zeroPos = zeroPos;
            this->moves = moves;
            this->goal = goal;
            this->heuristicValue = 0;
            this->heuristicNeeded = heuristicNeeded;

            if(heuristicNeeded)
                calculateHeuristic(this->board);
        }

        bool isLegalMove(char direction) {
            pii coordinate = make_pair(this->zeroPos.first + this->directions[direction].first, 
                                    this->zeroPos.second + this->directions[direction].second);

            return coordinate.first >= 0 && coordinate.first < 3 &&
                coordinate.second >= 0 && coordinate.second < 3;
        }

        Puzzle movePuzzle(char direction) {
            pii targetCoordinate = make_pair(this->zeroPos.first + this->directions[direction].first, 
                                    this->zeroPos.second + this->directions[direction].second);

            char target = this->board[_2dTo1dCoordinate(targetCoordinate.first, targetCoordinate.second)];
            string newBoard = this->board;
            newBoard[_2dTo1dCoordinate(targetCoordinate.first, targetCoordinate.second)] = '0';
            newBoard[_2dTo1dCoordinate(zeroPos.first, zeroPos.second)] = target;

            if(this->heuristicNeeded)
                return Puzzle(newBoard, true, targetCoordinate, this->moves + direction, goal);
            
            return Puzzle(newBoard, false, targetCoordinate, this->moves + direction, goal);
        }

        string getBoard() {
            return this->board;
        }

        static pii searchZeroPos(string board) {
            int zeroPosIn1d = 0;
            for(int i=0; i<board.length(); i++)
                if(board[i] == '0') {
                    zeroPosIn1d = i;
                    break;
                }
            
            return _1dTo2dCoordinate(zeroPosIn1d);
        }

        friend std::ostream& operator<<(std::ostream &out, const Puzzle &a) {
            string ret = "";
            if(a.heuristicNeeded)
                ret += "Heuristic value: " + to_string(a.heuristicValue) + '\n';

            ret += "Board:\n";
            for(int i=0; i<3; i++) {
                for(int j=0; j<3; j++) {
                    ret += a.board[Puzzle::_2dTo1dCoordinate(i, j)];
                    if(j < 2)
                        ret += ' ';
                }
                ret += '\n';
            }

            out << ret;
            return out;
        }

        static map<char, pii> setDirectionMap() {
            map<char, pii> tmp;
            tmp['u'] = make_pair(-1, 0);
            tmp['d'] = make_pair(1, 0);
            tmp['r'] = make_pair(0, 1);
            tmp['l'] = make_pair(0, -1);
            return tmp;
        }

        static vector<char> setMovements() {
            vector<char> tmp{'l', 'r', 'u', 'd'};
            return tmp;
        }

        static map<char, char> setReverseDirections() {
            map<char, char> tmp;
            tmp['u'] = 'd';
            tmp['d'] = 'u';
            tmp['l'] = 'r';
            tmp['r'] = 'l';
            return tmp;
        }
};

map<char, pii> Puzzle::directions = Puzzle::setDirectionMap();
vector<char> Puzzle::movements = Puzzle::setMovements();
map<char, char> Puzzle::reverseDirections = Puzzle::setReverseDirections();