#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

class punto {
public:
    punto(int a = 0, int b = 0) {
        x = a;
        y = b;
    }

    bool operator==(const punto &o) { return o.x == x && o.y == y; }

    punto operator+(const punto &o) { return punto(o.x + x, o.y + y); }

    int x, y;
};

class mapa {
public:
    mapa() {
        char t[8][8] = {
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 1, 0, 1, 1, 1, 0},
                {0, 0, 1, 0, 0, 0, 1, 0},
                {0, 1, 1, 0, 0, 0, 1, 0},
                {0, 0, 1, 1, 1, 1, 1, 1},
                {0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0}
        };
        w = h = 8;
        for (int r = 0; r < h; r++)
            for (int s = 0; s < w; s++)
                m[s][r] = t[r][s];
    }

    int operator()(int x, int y) { return m[x][y]; }

    char m[8][8];
    int w, h;
};

class nodo {
public:
    bool operator==(const nodo &o) { return pos == o.pos; }

    bool operator==(const punto &o) { return pos == o; }

    bool operator<(const nodo &o) { return dist + cost < o.dist + o.cost; }

    punto pos, parent;
    int dist, cost;
};

class aStar {
public:
    aStar() {
        cercano[0] = punto(-1, -1);
        cercano[1] = punto(1, -1);
        cercano[2] = punto(-1, 1);
        cercano[3] = punto(1, 1);
        cercano[4] = punto(0, -1);
        cercano[5] = punto(-1, 0);
        cercano[6] = punto(0, 1);
        cercano[7] = punto(1, 0);
    }

    int calcDist(punto &p) {
        int x = final.x - p.x, y = final.y - p.y;
        return (x * x + y * y);
    }

    bool isValid(punto &p) {
        return (p.x > -1 && p.y > -1 && p.x < m.w && p.y < m.h);
    }

    bool existPoint(punto &p, int cost) {
        list<nodo>::iterator i;
        i = find(closed.begin(), closed.end(), p);
        if ( i != closed.end()) {
            if ((*i).cost + (*i).dist < cost ) return true;
            else {
                closed.erase(i);
                return false;
            }
        }
        i = std::find(open.begin(), open.end(), p);
        if ( i != open.end()) {
            if ((*i).cost + (*i).dist < cost ) return true;
            else {
                open.erase(i);
                return false;
            }
        }
        return false;
    }

    bool fillOpen(nodo &n) {
        int stepCost, nc, dist;
        punto neighbour;

        for (int x = 0; x < 8; x++) {
            stepCost = x < 4 ? 1 : 1;
            neighbour = n.pos + cercano[x];
            if ( neighbour == final ) return true;

            if ( isValid(neighbour) && m(neighbour.x, neighbour.y) != 1 ) {
                nc = stepCost + n.cost;
                dist = calcDist(neighbour);
                if ( !existPoint(neighbour, nc + dist)) {
                    nodo m;
                    m.cost = nc;
                    m.dist = dist;
                    m.pos = neighbour;
                    m.parent = n.pos;
                    open.push_back(m);
                }
            }
        }
        return false;
    }

    bool search(punto &s, punto &e, mapa &mp) {
        nodo n;
        final = e;
        inicio = s;
        m = mp;
        n.cost = 0;
        n.pos = s;
        n.parent = 0;
        n.dist = calcDist(s);
        open.push_back(n);
        while (!open.empty()) {
            nodo n = open.front();
            open.pop_front();
            closed.push_back(n);
            if ( fillOpen(n)) return true;
        }
        return false;
    }

    int path(list<punto> &path) {
        path.push_front(final);
        int cost = 1 + closed.back().cost;
        path.push_front(closed.back().pos);
        punto parent = closed.back().parent;

        for (list<nodo>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++) {
            if ((*i).pos == parent && !((*i).pos == inicio)) {
                path.push_front((*i).pos);
                parent = (*i).parent;
            }
        }
        path.push_front(inicio);
        return cost;
    }

    mapa m;
    punto final, inicio;
    punto cercano[8];
    list<nodo> open;
    list<nodo> closed;
};

int main(int argc, char *argv[]) {
    mapa m;
    punto s(5, 4), e(6, 7);
    aStar as;

    if ( as.search(s, e, m)) {
        list<punto> path;
        int c = as.path(path);
        for (int y = -1; y < 9; y++) {
            for (int x = -1; x < 9; x++) {
                if ( x < 0 || y < 0 || x > 7 || y > 7 || m(x, y) == 1 )
                    cout << char(0xdb);
                else {
                    if ( find(path.begin(), path.end(), punto(x, y)) != path.end())
                        cout << "x";
                    else cout << ".";
                }
            }
            cout << "\n";
        }

        cout << "\nPath cost " << c << ": ";
        for (list<punto>::iterator i = path.begin(); i != path.end(); i++) {
            cout << "(" << (*i).x << ", " << (*i).y << ") ";
        }
    }
    cout << "\n\n";
    return 0;
}