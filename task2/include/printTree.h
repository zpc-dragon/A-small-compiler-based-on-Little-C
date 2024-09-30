#pragma once
#include"func.h"
class Canvas {
    public:
    static const int HEIGHT = 10;
    static const int WIDTH = 80;
    static char buffer[HEIGHT][WIDTH + 1];

    static void draw() {
        cout << endl;
        for (int i = 0; i < HEIGHT; i++) {
            buffer[i][WIDTH] = '\0';
            cout << buffer[i] << endl;
        }
        cout << endl;
    }

    static void put(int r, int c, const string& s) {
        int len = s.length();
        int idx = 0;
        for (int i = c; (i < WIDTH) && (idx < len); i++)
            buffer[r][i] = s[idx++];
    }
    static void put(int r, int c, char ch, int num) {
        while (num > 0 && c < WIDTH)
            buffer[r][c++] = ch, num--;
    }

    static void resetBuffer() {
        for (int i = 0; i < HEIGHT; i++)
            memset(buffer[i], ' ', WIDTH);
    }
};

char Canvas::buffer[Canvas::HEIGHT][Canvas::WIDTH + 1];

class BinaryTreeGui {
    public:

    static void show(node* root) {
        const int widthZoom = 3;
        printf("width zoom: %d\n", widthZoom);
        Canvas::resetBuffer();
        queue<node*> q;
        q.push(root);
        int x, y;
        string  token;
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            bool l = (p->firstchild != nullptr);
            bool r = (p->nextsibling != nullptr);
            x = p->x, y = p->y, token = (p->token);
            Canvas::put(2 * y, widthZoom * x, token);
            if (l) {
                q.push(p->firstchild);
                Canvas::put(2 * y + 1, widthZoom * p->firstchild->x, '_',
                    widthZoom * (x - p->firstchild->x) + token.length() / 2);
            }
            if (r) {
                q.push(p->nextsibling);
                Canvas::put(2 * y + 1, widthZoom * x, '_',
                    widthZoom * (p->nextsibling->x - x) + (p->nextsibling->token).length());
            }
            if (l || r)
                Canvas::put(2 * y + 1, widthZoom * x + token.length() / 2, "|");
        }
        Canvas::draw();
    }
    static void show2(node* root) {
        const int widthZoom = 2;
        printf("width zoom: %d\n", widthZoom);
        Canvas::resetBuffer();
        queue<node*> q;
        q.push(root);
        int x, y;
        string token;
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            x = p->x, y = p->y, token = p->token;
            Canvas::put(2 * y, widthZoom * x, (token));
            if (p->firstchild != nullptr) {
                q.push(p->firstchild);
                Canvas::put(2 * y + 1, widthZoom * ((p->firstchild->x + x) / 2), '/', 1);
            }
            if (p->nextsibling != nullptr) {
                q.push(p->nextsibling);
                Canvas::put(2 * y + 1, widthZoom * ((x + p->nextsibling->x) / 2) + 1, '\\', 1);
            }
        }
        Canvas::draw();
    }

    static void destroy(node* root) {
        if (root == nullptr)
            return;
        destroy(root->firstchild);
        destroy(root->nextsibling);
        delete root;
        root = nullptr;
    }

    private:


    static void initX(node* p, int& x) {
        if (p == nullptr)
            return;
        initX(p->firstchild, x);
        p->x = x++;
        initX(p->nextsibling, x);
    }
    static void initY(node* root) {
        if (root == nullptr)
            return;

        typedef pair<node*, int> Node;

        root->y = 1;

        queue<Node> q;
        q.push(Node(root, root->y));
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            if (p.first->firstchild != nullptr) {
                p.first->firstchild->y = p.second + 1;
                q.push(Node(p.first->firstchild, p.second + 1));
            }
            if (p.first->nextsibling != nullptr) {
                p.first->nextsibling->y = p.second + 1;
                q.push(Node(p.first->nextsibling, p.second + 1));
            }
        }
    }

    static void initCoordinate(node* root) {
        int x = 0;
        initX(root, x);
        initY(root);
    }

    // print info of tree nodes
    static void inorder(node* p) {
        if (p == nullptr)
            return;
        inorder(p->firstchild);
        printf("token=%d, x=%d, y=%d\n", p->token, p->x, p->y);
        inorder(p->nextsibling);
    }
};
