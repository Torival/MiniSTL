
#include "alloc.h"

MINISTL_NAMESPACE_BEGIN

#define sz(node) (node == 0 ? 0 : node->size)

template <class T> 
struct sbTreeNode {
    typedef sbTreeNode* base_ptr;

    base_ptr left;
    base_ptr right;
    int size;

    T val;
    sbTreeNode(T _val) :left(0), right(0), size(1) { val = _val; }

    static base_ptr getmax(base_ptr cur){
        while(cur->left != NULL)
            cur = cur->left;
        return cur;
    }

    static base_ptr getmin(base_ptr cur) {
        while(cur->right != NULL)
            cur = cur->right;
        return cur;
    }
};

template <class T, class Alloc = default_alloc>
class sbtree {
public:
    typedef typename sbTreeNode<T>::base_ptr base_ptr;
    
    sbtree();
    ~sbtree();
    
    bool insert(T val)
    {
        return insert_aux(root, val);
    }
    
    bool erase(T val)
    {   return erase_aux(root, val);    }
    
    bool find(T val)
    {   return find_aux(root, val);     }
    
    // 返回在第 k 位置上的结点
    T select(int k)
    {   return select_aux(root, k);     }
    
    // 返回val在以root为根的树中的排名
    int rank(T val)
    {   return rank_aux(root, val);     }
    
    T pred(T val)
    {   return pred_aux(root, val);     }

    T succ(T val)
    {   return succ_aux(root, val);     }
    
    
private:
    bool rightRotate(base_ptr& cur);
    bool leftRotate(base_ptr& cur);

    bool insert_aux(base_ptr& cur, T val);
    bool erase_aux(base_ptr& cur, T val);
    bool find_aux(base_ptr cur, T val);
    T select_aux(base_ptr cur, int k);
    int rank_aux(base_ptr cur, T val);
    T pred_aux(base_ptr cur, T val);
    T succ_aux(base_ptr cur, T val);
    void destory(base_ptr cur);

private:

    base_ptr root;
};

template <class T, class Alloc>
sbtree<T, Alloc>::sbtree() :root(0) {

}

template <class T, class Alloc>
sbtree<T, Alloc>::~sbtree () {
    destory(root);
    root = NULL;
}

//      cur: x                                    cur: y
//        x                                         y
//       / \     ------------>rightRotate          / \
//      y   c                                     a   x
//     / \       leftRotate<-------------            / \
//    a   b                                         b   c

template <class T, class Alloc>
bool sbtree<T, Alloc>::rightRotate(base_ptr& cur) {
    
    base_ptr temp = cur->left;
    cur->left = temp->right;
    temp->right = cur;

    temp->size = cur->size;
    cur->size = sz(cur->left) + sz(cur->right) + 1;
    cur = temp;

    return true;
}

template <class T, class Alloc>
bool sbtree<T, Alloc>::leftRotate(base_ptr& cur) {
    base_ptr temp = cur->right;
    cur->right = temp->left;
    temp->left = cur;

    temp->size = cur->size;
    cur->size = sz(cur->left) + sz(cur->right) + 1;
    cur = temp;

    return true;
}

template <class T, class Alloc>
bool sbtree<T, Alloc>::insert_aux(base_ptr& cur, T val){
    
    if (cur == NULL) {
        cur = new sbTreeNode<T>(val);
    } else {
        cur->size++;
        if (cur->val > val){

            insert_aux(cur->left, val);
            
            if (cur->left != NULL &&
               ( (sz(cur->left->left) > sz(cur->right))  || 
                 (sz(cur->left->right) > sz(cur->right)) )) {
                //cout << "\nrr    ----: " << cur->val << "\n";
                rightRotate(cur);
            }
        } else {

            insert_aux(cur->right, val);
            
            if (cur->right != NULL && 
               ( (sz(cur->right->right) > sz(cur->left)) ||
                 (sz(cur->right->left) > sz(cur->left))  )){
                //cout << "\nlr    ----: " << cur->val << "\n";
                leftRotate(cur);
            }
        }
    }
    return true;
}

template <class T, class Alloc>
bool sbtree<T, Alloc>::erase_aux(base_ptr& cur, T val){
    if(cur == NULL)
        return false;

    cur->size -= 1;
    if(cur->val == val){
        // 右支为空
        if(cur->right == NULL){
            cur = cur->left;
        // 左支为空
        } else if(cur->left == NULL) {
            cur = cur->right;
        // 找到右子树中的最小节点，替换删除了的父节点
        } else{
            sbTreeNode<T> temp = *sbTreeNode<T>::getmin(cur->right);
            erase_aux(cur->right, temp.val);
            temp.left = cur->left;
            temp.right = cur->right;
            *cur = temp;
        }

    }

    if(cur->val > val)
        erase_aux(cur->right, val);
    else
        erase_aux(cur->left, val);
    return true;
}

template <class T, class Alloc>
bool sbtree<T, Alloc>::find_aux(base_ptr cur, T val) {
    while (cur != NULL) {
        if (cur->val == val)
            return true;
        else if (cur->val > val)
            cur = cur->left;
        else
            cur = cur->right;
    }

    return false;
}

template <class T, class Alloc>
T sbtree<T, Alloc>::select_aux(base_ptr cur, int k){
    //Select K 大于SBT的个数
    if (cur->size < k)
        return -1;

    if (sz(cur->left) + 1 > k)
        return select_aux(cur->left, k);
    else if (sz(cur->left) + 1 < k)
        return select_aux(cur->right, k - sz(cur->left) -1);
    else
        return cur->val;

    return -1;
}

template <class T, class Alloc>
int sbtree<T, Alloc>::rank_aux(base_ptr cur, T val){
    if (cur->val == val)
        return sz(cur->left) + 1;
    else if (cur->val > val)
        return rank_aux(cur->left, val);
    else
        return sz(cur->left) + 1 + rank_aux(cur->right, val);

    return -1;
}


template <class T, class Alloc>
void sbtree<T, Alloc>::destory(base_ptr cur){
    if (cur != NULL) {
        destory(cur->left);
        destory(cur->right);
        delete cur;
    }
}

MINISTL_NAMESPACE_END
