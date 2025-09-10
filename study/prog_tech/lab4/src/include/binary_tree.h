#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <queue>
#include <cstring>
#include <cstring>


template<typename T>
class TreeNode {
public:
    T data;                   
    TreeNode<T>* left;         
    TreeNode<T>* right;        
    
    
    TreeNode(const T& value) : data(value), left(nullptr), right(nullptr) {}
};


template<typename T>
class BinaryTree {
private:
    TreeNode<T>* root;         
    TreeNode<T>* insert_recursive(TreeNode<T>* node, const T& value);
    bool search_recursive(TreeNode<T>* node, const T& value) const;
    void preorder_recursive(TreeNode<T>* node) const;      
    void postorder_recursive(TreeNode<T>* node) const;     
    void inorder_recursive(TreeNode<T>* node) const;       
    void destroy_tree(TreeNode<T>* node);
    TreeNode<T>* copy_tree(TreeNode<T>* node);
    void print_tree_structure(TreeNode<T>* node, int level, char prefix) const;
    void print_tree_recursive(TreeNode<T>* node, const std::string& indent, bool isLast) const;
    
public:

    BinaryTree();
    

    BinaryTree(const BinaryTree<T>& other);
    

    BinaryTree<T>& operator=(const BinaryTree<T>& other);
    

    ~BinaryTree();
    

    void insert(const T& value);                          
    bool search(const T& value) const;                    
    void preorder_traversal() const;                      
    void postorder_traversal() const;                    
    void inorder_traversal() const;                        
    void print_tree() const;                               
    bool is_empty() const;                                
    void clear();                                         
};



template<typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& other) : root(nullptr) {
    if (other.root != nullptr) {
        root = copy_tree(other.root);
    }
}

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& other) {
    if (this != &other) {
        clear();
        if (other.root != nullptr) {
            root = copy_tree(other.root);
        }
    }
    return *this;
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
    clear();
}

template<typename T>
void BinaryTree<T>::insert(const T& value) {
    root = insert_recursive(root, value);
}

template<typename T>
TreeNode<T>* BinaryTree<T>::insert_recursive(TreeNode<T>* node, const T& value) {

    if (node == nullptr) {
        return new TreeNode<T>(value);
    }
    
    if (node->data > value) {
        node->right = insert_recursive(node->right, value);
    }

    else if (node->data < value) {
        node->left = insert_recursive(node->left, value);
    }

    
    return node;
}

template<typename T>
bool BinaryTree<T>::search(const T& value) const {
    return search_recursive(root, value);
}

template<typename T>
bool BinaryTree<T>::search_recursive(TreeNode<T>* node, const T& value) const {

    if (node == nullptr) {
        return false;
    }
    
    
    if (value == node->data) {
        return true;
    }
    

    if (node->data > value) {
        return search_recursive(node->right, value);
    }

    else {
        return search_recursive(node->left, value);
    }
}

template<typename T>
void BinaryTree<T>::preorder_traversal() const {
    std::cout << "Обход в нисходящем порядке (Корень -> Левое -> Правое): ";
    preorder_recursive(root);
    std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::preorder_recursive(TreeNode<T>* node) const {
    if (node != nullptr) {
        std::cout << node->data << " ";    
        preorder_recursive(node->left);    
        preorder_recursive(node->right); 
    }
}

template<typename T>
void BinaryTree<T>::postorder_traversal() const {
    std::cout << "Обход в восходящем порядке (Левое -> Правое -> Корень): ";
    postorder_recursive(root);
    std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::postorder_recursive(TreeNode<T>* node) const {
    if (node != nullptr) {
        postorder_recursive(node->left);   
        postorder_recursive(node->right);  
        std::cout << node->data << " ";    
    }
}

template<typename T>
void BinaryTree<T>::inorder_traversal() const {
    std::cout << "Симметричный обход (Левое -> Корень -> Правое): ";
    inorder_recursive(root);
    std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::inorder_recursive(TreeNode<T>* node) const {
    if (node != nullptr) {
        inorder_recursive(node->left);    
        std::cout << node->data << " ";    
        inorder_recursive(node->right);    
    }
}

template<typename T>
void BinaryTree<T>::print_tree() const {
    if (root == nullptr) {
        std::cout << "Дерево пустое" << std::endl;
        return;
    }
    
    std::cout << "Структура дерева:" << std::endl;
    std::cout << "." << std::endl; 
    print_tree_recursive(root, "", true);
}

template<typename T>
void BinaryTree<T>::print_tree_recursive(TreeNode<T>* node, const std::string& indent, bool isLast) const {
    if (node != nullptr) {
        std::cout << indent;
        
        if (isLast) {
            std::cout << "└── ";
        } else {
            std::cout << "├── ";
        }
        
        std::cout << node->data << std::endl;
        

        std::string newIndent = indent;
        if (isLast) {
            newIndent += "    ";
        } else {
            newIndent += "│   ";
        }
        

        bool hasLeft = (node->left != nullptr);
        bool hasRight = (node->right != nullptr);
        

        if (hasLeft && hasRight) {
            print_tree_recursive(node->left, newIndent, false);
            print_tree_recursive(node->right, newIndent, true);
        } else if (hasLeft) {

            print_tree_recursive(node->left, newIndent, true);
        } else if (hasRight) {

            print_tree_recursive(node->right, newIndent, true);
        }
    }
}

template<typename T>
void BinaryTree<T>::print_tree_structure(TreeNode<T>* node, int level, char prefix) const {
    if (node != nullptr) {

        print_tree_structure(node->right, level + 1, 'R');
        

        for (int i = 0; i < level; i++) {
            if (i == level - 1) {
                if (prefix == 'L') {
                    std::cout << "└── ";
                } else {
                    std::cout << "┌── ";
                }
            } else {
                std::cout << "│   ";
            }
        }
        
        if (level == 0) {
            std::cout << node->data << " (корень)" << std::endl;
        } else {
            std::cout << node->data << std::endl;
        }
        

        print_tree_structure(node->left, level + 1, 'L');
    }
}

template<typename T>
bool BinaryTree<T>::is_empty() const {
    return root == nullptr;
}

template<typename T>
void BinaryTree<T>::clear() {
    destroy_tree(root);
    root = nullptr;
}

template<typename T>
void BinaryTree<T>::destroy_tree(TreeNode<T>* node) {
    if (node != nullptr) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }
}

template<typename T>
TreeNode<T>* BinaryTree<T>::copy_tree(TreeNode<T>* node) {
    if (node == nullptr) {
        return nullptr;
    }
    
    TreeNode<T>* new_node = new TreeNode<T>(node->data);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    return new_node;
}

// Специализации для char*
template<>
TreeNode<char*>* BinaryTree<char*>::insert_recursive(TreeNode<char*>* node, char* const& value) {
    if (node == nullptr) {
        return new TreeNode<char*>(value);
    }
    
    int cmp = strcmp(node->data, value);
    if (cmp > 0) {
        node->right = insert_recursive(node->right, value);
    }
    else if (cmp < 0) {
        node->left = insert_recursive(node->left, value);
    }
    
    return node;
}

template<>
bool BinaryTree<char*>::search_recursive(TreeNode<char*>* node, char* const& value) const {
    if (node == nullptr) {
        return false;
    }
    
    int cmp = strcmp(value, node->data);
    if (cmp == 0) {
        return true;
    }
    
    if (strcmp(node->data, value) > 0) {
        return search_recursive(node->right, value);
    }
    else {
        return search_recursive(node->left, value);
    }
}

#endif // BINARY_TREE_H
