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
    int to_price;               // cost of the path to the cell
    int goal_price;             // estimated cost of the path to the keymaker
    int through;                // estimated cost of the path through the cell
    vector<Cell*> ancestors;    // the vector of ancestors cells (from initial to current)
    int x, y;                   // coordinates of the cell
    bool danger = false;        // danger indicator
    bool visited = false;       // visited indicator
    static int x_km, y_km;      // coordinates of the keymaker

    // constructor for creating the cell
    Cell(int x, int y, int tp) : x(x), y(y), to_price(tp) {
        goal_price = abs(x - x_km) + abs(y - y_km);   // heuristics is a Manhatten distance
        through = to_price + goal_price;
    }

    // static function for setting the keymaker coordinates
    static void set_km_coord(int x, int y) {
        x_km = x;
        y_km = y;
    }

    // function for going to the current cell from the given cell
    void gofrom(Cell* init_cell, vector<vector<Cell>>& field) const {
        
        // going back to common ancestor
        while (!is_ancestor(init_cell)) {
            init_cell = init_cell->parent();
            cout << "m " << init_cell->x  << " " << init_cell->y << endl;
            getinfo(field);
        }
        
        // going from the common ancestor to the current cell
        bool flag = false;
        for (Cell* mid : ancestors) {
            if (mid->x == init_cell->x && mid->y == init_cell->y) {
                flag = true;
            } else if (flag) {
                cout << "m " << mid->x  << " " << mid->y << endl;
                getinfo(field);
            }
        }
        cout << "m " << x  << " " << y << endl;
        return;
    }

    // checking if the given cell an ancestor of the current cell or not
    bool is_ancestor(Cell* c) const {
        for (Cell* a : ancestors) {
            if (a == c) return true;
        }
        return false;
    }

    // getter of the parent (the last ancestor)
    Cell* parent() const {
        return ancestors.back();
    }

    // updating the to_price and ancestors in case of finding the cheaper path
    void check(int new_to_price, Cell* par) {
        if (new_to_price < to_price) {
            to_price = new_to_price;
            through = to_price + goal_price;
            ancestors = par->ancestors;
            ancestors.push_back(par);
        }
    }

    // choosing the next cell to move
    static Cell* choose(vector<Cell*>& neighbors) {
        if (neighbors.size() == 0) return nullptr;
        Cell* res = nullptr;
        int res_ind = -1;
        int minn_thr = __INT_MAX__;
        int minn_goal = __INT_MAX__;
        int i = 0;
        for (Cell* c : neighbors) {
            // choose the cheapest through price
            if (c->through < minn_thr) {
                res = c;
                res_ind = i;
                minn_thr = c->through;
                minn_goal = c->goal_price;
            // or choose the cheapest goal_price in case of equal through
            } else if (c->through == minn_thr && c->goal_price < minn_goal) {
                res = c;
                res_ind = i;
                minn_thr = c->through;
                minn_goal = c->goal_price;
            }
            i++;
        }
        neighbors.erase(neighbors.cbegin() + res_ind);
        return res;
    }
};

// setting the default coordinates of the keymaker
// (we can't leave the static member empty)
int Cell::x_km = 0;
int Cell::y_km = 0;

// adding the cell to the "neighbors" vector (we need elements to be unique and ordered)
void add(vector<Cell*>& neighbors, Cell* cell) {
    for (Cell* c : neighbors) {
        if (c->x == cell->x && c->y == cell->y) {
            return;
        }
    }
    neighbors.push_back(cell);
}

int Astar(const int& mode, const int& x_km, const int& y_km) {

    // say the coordinates of the keymaker to the Cell structure
    Cell::set_km_coord(x_km, y_km);

    // creating a matrix representing the field
    vector<vector<Cell>> field;
    for (int i = 0; i < 9; i++) {
        vector<Cell> row;
        for (int j = 0; j < 9; j++) {
            row.push_back(Cell(i, j, __INT_MAX__));  // initially to_price should be big
        }
        field.push_back(row);
    }

    vector<Cell*> neighbors;          // vector of the cells we can move to
    Cell* cell_now = &field[0][0];    // pointer to the current cell
    cell_now->to_price = 0;           // to_price of the initial cell is 0
    cell_now->visited = true;         // mark the initial cell as already visited
    cout << "m 0 0" << endl;          // request the information about the environmnet of the initial cell
    getinfo(field);

    while (true) {

        // consider step up (if the cell is within the map boundary, not danger and not visited)
        if (cell_now->x-1 >= 0 && !field[cell_now->x-1][cell_now->y].danger && !field[cell_now->x-1][cell_now->y].visited) {
            // add the cell we can move to
            add(neighbors, &field[cell_now->x-1][cell_now->y]);
            // updating the price to this cell
            field[cell_now->x-1][cell_now->y].check(cell_now->to_price + 1, cell_now);
        }

        // consider step down
        if (cell_now->x+1 <= 8 && !field[cell_now->x+1][cell_now->y].danger && !field[cell_now->x+1][cell_now->y].visited) {
            add(neighbors, &field[cell_now->x+1][cell_now->y]);
            field[cell_now->x+1][cell_now->y].check(cell_now->to_price + 1, cell_now);
        }

        // consider step left
        if (cell_now->y-1 >= 0 && !field[cell_now->x][cell_now->y-1].danger && !field[cell_now->x][cell_now->y-1].visited) {
            add(neighbors, &field[cell_now->x][cell_now->y-1]);
            field[cell_now->x][cell_now->y-1].check(cell_now->to_price + 1, cell_now);
        }

        // consider step right
        if (cell_now->y+1 <= 8 && !field[cell_now->x][cell_now->y+1].danger && !field[cell_now->x][cell_now->y+1].visited) {
            add(neighbors, &field[cell_now->x][cell_now->y+1]);
            field[cell_now->x][cell_now->y+1].check(cell_now->to_price + 1, cell_now);
        }

        
        Cell* new_cell = Cell::choose(neighbors);   // choose the step to move to
        if (new_cell == nullptr) return -1;         // if we have no cells to move to, then the map is unsolvable
        new_cell->gofrom(cell_now, field);          // going to new cell from current cell
        cell_now = new_cell;                        // updating the pointer to the current cell
        cell_now->visited = true;                   // mark new cell as visited
        getinfo(field);                             // request the information about the new cell

        // if we found the keymaker, then return the final path
        if (cell_now->x == x_km && cell_now->y == y_km) return cell_now->to_price;
    }
}

int main() {
    int mode, x_km, y_km;
    cin >> mode >> x_km >> y_km;
    int res = Astar(mode, x_km, y_km);
    cout << "e " << res << endl;
    return 0;
}
