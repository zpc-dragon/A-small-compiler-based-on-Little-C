#include <queue>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    int x, y;
    TreeNode* left, * right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr), x(-1), y(-1) {}
};

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
    static TreeNode* create(string& s) {
        // discard the '[]'
        s = s.substr(1, s.size() - 2);

        // change s into ["1", ...,"2"]
        auto v = split(s, ",");

        // create binary tree
        TreeNode* root = nullptr;
        innerCreate(v, 0, root);

        // init x and y of tree nodes
        initCoordinate(root);

        return root;
    }

    static void show(TreeNode* root) {
        const int widthZoom = 3;
        printf("width zoom: %d\n", widthZoom);
        Canvas::resetBuffer();
        queue<TreeNode*> q;
        q.push(root);
        int x, y, val;
        string sval;
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            bool l = (p->left != nullptr);
            bool r = (p->right != nullptr);
            x = p->x, y = p->y, val = p->val, sval = to_string(p->val);
            Canvas::put(2 * y, widthZoom * x, sval);
            if (l) {
                q.push(p->left);
                Canvas::put(2 * y + 1, widthZoom * p->left->x, '_',
                    widthZoom * (x - p->left->x) + sval.length() / 2);
            }
            if (r) {
                q.push(p->right);
                Canvas::put(2 * y + 1, widthZoom * x, '_',
                    widthZoom * (p->right->x - x) + to_string(p->right->val).length());
            }
            if (l || r)
                Canvas::put(2 * y + 1, widthZoom * x + sval.length() / 2, "|");
        }
        Canvas::draw();
    }
    static void show2(TreeNode* root) {
        const int widthZoom = 2;
        printf("width zoom: %d\n", widthZoom);
        Canvas::resetBuffer();
        queue<TreeNode*> q;
        q.push(root);
        int x, y, val;
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            x = p->x, y = p->y, val = p->val;
            Canvas::put(2 * y, widthZoom * x, to_string(val));
            if (p->left != nullptr) {
                q.push(p->left);
                Canvas::put(2 * y + 1, widthZoom * ((p->left->x + x) / 2), '/', 1);
            }
            if (p->right != nullptr) {
                q.push(p->right);
                Canvas::put(2 * y + 1, widthZoom * ((x + p->right->x) / 2) + 1, '\\', 1);
            }
        }
        Canvas::draw();
    }

    static void destroy(TreeNode* root) {
        if (root == nullptr)
            return;
        destroy(root->left);
        destroy(root->right);
        delete root;
        root = nullptr;
    }

    private:
    static void innerCreate(vector<string>& v, size_t idx, TreeNode*& p) {
        if (idx >= v.size() || v[idx] == "null")
            return;
        p = new TreeNode(stoi(v[idx]));
        innerCreate(v, 2 * idx + 1, p->left);
        innerCreate(v, 2 * idx + 2, p->right);
    }

    static void replaceAll(string& s, const string& oldChars, const string& newChars) {
        int pos = s.find(oldChars);
        while (pos != string::npos) {
            s.replace(pos, oldChars.length(), newChars);
            pos = s.find(oldChars);
        }
    }

    static vector<string> split(string& s, const string& token) {
        replaceAll(s, token, " ");
        vector<string> result;
        stringstream ss(s);
        string buf;
        while (ss >> buf)
            result.push_back(buf);
        return result;
    }

    static void initX(TreeNode* p, int& x) {
        if (p == nullptr)
            return;
        initX(p->left, x);
        p->x = x++;
        initX(p->right, x);
    }
    static void initY(TreeNode* root) {
        if (root == nullptr)
            return;

        typedef pair<TreeNode*, int> Node;

        root->y = 1;

        queue<Node> q;
        q.push(Node(root, root->y));
        while (!q.empty()) {
            auto p = q.front();
            q.pop();
            if (p.first->left != nullptr) {
                p.first->left->y = p.second + 1;
                q.push(Node(p.first->left, p.second + 1));
            }
            if (p.first->right != nullptr) {
                p.first->right->y = p.second + 1;
                q.push(Node(p.first->right, p.second + 1));
            }
        }
    }

    static void initCoordinate(TreeNode* root) {
        int x = 0;
        initX(root, x);
        initY(root);
    }

    // print info of tree nodes
    static void inorder(TreeNode* p) {
        if (p == nullptr)
            return;
        inorder(p->left);
        printf("val=%d, x=%d, y=%d\n", p->val, p->x, p->y);
        inorder(p->right);
    }
};

int main(int argc, char* argv[]) {
    string s = "[512,46, 7453,35, 6,26,null,-1,null,9,null]";
    // string s = "[6,2,8,0,4,7,9,null,null,3,5]";
    // string s(argv[1]);
    auto root = BinaryTreeGui::create(s);
    BinaryTreeGui::show(root);
    BinaryTreeGui::show2(root);
    BinaryTreeGui::destroy(root);
}
