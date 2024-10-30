#include <iostream>
#include <vector>

using namespace std;

// input processing
template<typename T>
void getinfo(vector<vector<T>>& field) {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y;
        char info;
        cin >> x >> y >> info;
        if (info == 'P' || info == 'A' || info == 'S') {
            field[x][y].danger = true;
        }
    }
}

// structure Cell stands for the cell of the field
struct Cell {
    Cell* parent = nullptr;       // pointer to the parent cell
    int x, y;                     // coordinates of the cell
    bool danger = false;          // danger indicator
    bool visited = false;
    vector<Cell*> wentto;         // the cells that we already went to from the current cell
    static int x_km, y_km;        // coordinates of the keymaker

    // constructor for creating the cell
    Cell(int x, int y) : x(x), y(y) {}

    // static function for setting the keymaker coordinates
    static void set_km_coord(int x, int y) {
        x_km = x;
        y_km = y;
    }

    // check whether we went to the given cell from the current one or not
    bool iswentto(Cell* p) {
        for (Cell* k : wentto) {
            if (k == p) return true;
        }
        return false;
    }
};

// setting the default coordinates of the keymaker
// (we can't leave the static member empty)
int Cell::x_km = 0;
int Cell::y_km = 0;

int backtrack(const int& mode, const int& x_km, const int& y_km) {

    Cell::set_km_coord(x_km, y_km);

    // creating a matrix representing the field
    vector<vector<Cell>> field;
    for (int i = 0; i < 9; i++) {
        vector<Cell> row;
        for (int j = 0; j < 9; j++) {
            row.push_back(Cell(i, j));
        }
        field.push_back(row);
    }

    vector<Cell*> neighbors;          // vector of the cells we can move to
    Cell* cell_now = &field[0][0];    // pointer to the current cell
    int current_cost = 0;
    int minn = __INT_MAX__;
    cout << "m 0 0" << endl;          // request the information about the environmnet of the initial cell
    getinfo(field);

    while (true) {

        if (cell_now->x == x_km && cell_now->y == y_km) {
            if (current_cost < minn) {
                minn = current_cost;
            }
        }

        // preparing for moving
        bool moved = false;
        Cell* newcell;

        // consider step up (if the cell is within the map boundary, not danger, not visited from the current cell, and has bigger price)
        if (cell_now->x-1 >= 0 && !field[cell_now->x-1][cell_now->y].visited && !field[cell_now->x-1][cell_now->y].danger && !cell_now->iswentto(&field[cell_now->x-1][cell_now->y])) {
            // if we still did not choose the cell to move, choose this one
            if (!moved) {
                newcell = &field[cell_now->x-1][cell_now->y];
                moved = true;
            }
        }

        // consider step down (-||-)
        if (cell_now->x+1 <= 8 && !field[cell_now->x+1][cell_now->y].visited && !field[cell_now->x+1][cell_now->y].danger && !cell_now->iswentto(&field[cell_now->x+1][cell_now->y])) {
            if (!moved) {
                newcell = &field[cell_now->x+1][cell_now->y];
                moved = true;
            }
        }

        // consider step left (-||-)
        if (cell_now->y-1 >= 0 && !field[cell_now->x][cell_now->y-1].visited && !field[cell_now->x][cell_now->y-1].danger && !cell_now->iswentto(&field[cell_now->x][cell_now->y-1])) {
            if (!moved) {
                newcell = &field[cell_now->x][cell_now->y-1];
                moved = true;
            }
        }

        // consider step right (-||-)
        if (cell_now->y+1 <= 8 && !field[cell_now->x][cell_now->y+1].visited && !field[cell_now->x][cell_now->y+1].danger && !cell_now->iswentto(&field[cell_now->x][cell_now->y+1])) {
            if (!moved) {
                newcell = &field[cell_now->x][cell_now->y+1];
                moved = true;
            }
        }

        if (!moved && cell_now->parent != nullptr) {                     // if we did not find the cell to move to and we have parent cell,
            if (cell_now->parent->danger) return -1;                     // if parent become danger (in case of taking the key), we are in the dead end
            cell_now->visited = false;
            cell_now = cell_now->parent;                                 // move to the parent cell
            cout << "am " << cell_now->x << " " << cell_now->y << endl;
            current_cost--;
            getinfo(field);
        } else if (!moved) {                                             // if the did not find the cell to move to and we have no parent (initial)
            if (minn == __INT_MAX__) return -1;                          // return the result
            return minn;
        } else {                                                         // if we found the cell to move to
            newcell->parent = cell_now;                                  // move there
            cell_now->wentto.push_back(newcell);
            cell_now = newcell;
            cell_now->visited = true;
            cell_now->wentto.push_back(cell_now->parent);
            cout << "bm " << cell_now->x << " " << cell_now->y << endl;
            current_cost++;
            getinfo(field);
        }
    }
}

int main() {
    int mode, x_km, y_km;
    cin >> mode >> x_km >> y_km;
    int res = backtrack(mode, x_km, y_km);
    cout << "e " << res << endl;
    return 0;
}