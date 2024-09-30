#include<bits/stdc++.h>
#define N 100
using namespace std;
typedef struct node {
    char data;		// 节点数据
    struct node* fir, * sib;		// 指向第一个子节点和下一个兄弟节点的指针
}TR;
typedef struct queue {
    int fa;		// 记录node节点的父节点在队列中的索引
    TR* node;		// 指向TR节点的指针
}QU;
#define N 100
TR* createTR(char* in, char* pre, int k);   //  创建一棵树
void showTR(TR* T);     // 展示一棵树   A(B,C(E,F))这样的形式
TR* destroy(TR* T);     // 销毁一棵树
void preorder(TR* T);   // 前序遍历
void postorder(TR* T);  // 后序遍历
void layer(TR* T);      // 层次遍历
int height(TR* T);      // 树的高度
int leaf(TR* T);        // 计算叶子的数量
void getpath1(TR* T);   // 打印根到叶子节点的所有路径(dfs)
void allpath(TR* T, char* path, int n);   // 配合getpath1完成递归
void getpath2(TR* T);   // 基于后序遍历的非递归找路径
void getpath3(TR* T);   // 基于层次遍历的非递归找路径
void longestPath(TR* T);    // 输出根节点到叶子节点所有路径中最长的那些路径 
void insert(TR* T, char s1, char s2);  // 在data为s1的节点下插入data为s2的子节点
// 若s1有子节点，则将s2放入s1的末尾；若s1没有子节点，则将s2作为新的节点插入
TR* delsub(TR* T, char s);      // 递归地销毁根节点data为s的子树
TR* lca(TR* T, char s1, char s2);   // 寻找s1和s2的最近公共祖先
void mirror(TR* T);     // 多叉树逆置


int main() {
    char pre[] = "ABEFCDGHIJ", in[] = "EFBCHIJGDA";  // 目标多叉树对应的二叉链的先序遍历和中序遍历
    TR* header = NULL;
    int length = strlen(pre);

    header = createTR(in, pre, length); // 创建一颗多叉树
    cout << "创建的树为:" << endl;
    showTR(header);
    cout << endl;

    cout << "前序遍历:" << endl;
    preorder(header);
    cout << endl;

    cout << "后序遍历:" << endl;
    postorder(header);
    cout << endl;

    cout << "层次遍历:" << endl;
    layer(header);
    cout << endl;

    cout << "树的高度为:" << endl;
    cout << height(header) << endl;

    cout << "树的叶子数量为:" << endl;
    cout << leaf(header) << endl;

    cout << "从根节点到子节点的所有路径(递归):" << endl;
    getpath1(header);
    cout << endl;

    // cout << "从根节点到子节点的所有路径(后序遍历):" << endl;
    // getpath2(header);
    // cout << endl;

    // cout << "从根节点到子节点的所有路径(层次遍历):" << endl;
    // getpath3(header);
    // cout << endl;

    // cout << "根节点到叶子节点的最长路径有：" << endl;
    // longestPath(header);
    // cout << endl;

    // cout << "插入新节点后的树:" << endl;
    // insert(header, 'E', 'X');
    // insert(header, 'A', 'K');
    // insert(header, 'I', 'L');
    // showTR(header);
    // cout << endl;

    // TR* p;
    // p = lca(header, 'X', 'F');
    // cout << "距离X和F最近的共同祖先是:" << p->data << endl;
    // p = lca(header, 'K', 'L');
    // cout << "距离K和L最近的共同祖先是:" << p->data << endl;
    // p = lca(header, 'L', 'H');
    // cout << "距离L和H最近的共同祖先是:" << p->data << endl;

    // cout << "删除I分支后，树为:" << endl;
    // header = delsub(header, 'I');
    // showTR(header);
    // cout << endl;

    // cout << "删除E分支后，树为:" << endl;
    // header = delsub(header, 'E');
    // showTR(header);
    // cout << endl;

    // mirror(header);
    // cout << "逆置后的多叉树为:" << endl;
    // showTR(header);
    // cout << endl;

    cout << "销毁树" << endl;
    destroy(header);

    return 0;
}



TR* createTR(char* in, char* pre, int k) {
    if (k <= 0) {
        return NULL;
    } else {
        TR* node = new TR;         // 创建一个新节点
        node->data = pre[0];      // 新节点的data为当前先序遍历的开头，也就是本层递归创建的树的根节点
        int i;
        for (i = 0; in[i] != pre[0]; ++i);     // 在中序遍历中寻找根节点，i代表根节点在中序遍历中的索引
        node->fir = createTR(in, pre + 1, i);	// 创建二叉链的左分支的根节点
        node->sib = createTR(in + i + 1, pre + i + 1, k - i - 1);	// 创建二叉链的右分支的根节点
        return node;
    }
}
void showTR(TR* T) {
    if (T) {
        cout << T->data;
        if (T->fir) {
            cout << "(";
            TR* p = T->fir;
            showTR(p);
            p = p->sib;
            while (p) {
                cout << ",";
                showTR(p);
                p = p->sib;
            }
            cout << ")";
        }
    }
}
TR* destroy(TR* T) {
    if (!T) {
        return NULL;
    } else {
        TR* p = T->fir, * p2;
        while (p) {
            p2 = p->sib;     // 因为p会被销毁，所以需要一个p2来保存指向的节点地址
            destroy(p);
            p = p2;
        }
        delete T;
        return NULL;
    }
}
void preorder(TR* T) {
    if (T) {
        cout << T->data << " ";
        preorder(T->fir);
        preorder(T->sib);
    }
}
void postorder(TR* T) {
    if (T) {
        postorder(T->fir);
        cout << T->data << " ";
        postorder(T->sib);
    }
}

void layer(TR* T) {
    TR* q[N], * p;       // 通过队列来完成层次遍历
    int front, rear;
    front = rear = 0;
    q[rear++] = T;     // 根节点先入队

    while (front != rear)       // 循环的结束条件为队列为空
    {
        // 基本逻辑很简单，每打印队首的元素，就将队首的所有节点全部入队
        p = q[front++];
        cout << p->data << " ";
        p = p->fir;

        while (p) {
            q[rear++] = p;
            p = p->sib;
        }
    }
}
int height(TR* T) {
    if (!T) {
        return 0;
    } else {
        int h1, h2;
        h1 = height(T->fir) + 1;       // 由于T的子节点比兄弟节点低一层，所以需要加一
        h2 = height(T->sib);
        return max(h1, h2);
    }
}

void getpath1(TR* T) {
    char path[N];
    path[0] = T->data;
    allpath(T, path, 1);
}

void allpath(TR* T, char* path, int n) {
    if (!T->fir) {
        for (int i = 0; i < n; ++i)
            cout << path[i] << " ";
        cout << endl;
    }
    if (T->fir) {
        path[n] = T->fir->data;
        allpath(T->fir, path, n + 1);
    }
    if (T->sib) {
        path[n - 1] = T->sib->data;
        allpath(T->sib, path, n);
    }
}
int leaf(TR* T) {
    if (!T)
        return 0;
    else if (!T->fir)
        return 1 + leaf(T->sib);	// 二叉链的结构特性导致需要把兄弟节点返回的个数加一
    else
        return leaf(T->fir) + leaf(T->sib);
}
