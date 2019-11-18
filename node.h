#ifndef PROJECT3_NODE_H
#define PROJECT3_NODE_H

#include <string>
#include <iostream>

class node {
private:
    std::string title="";
    int available=0;
    int rented=0;
    node* left=nullptr;
    node* right=nullptr;
public:
    node();
    node(std::string,int,int);
    std::string getTitle(){return title;};
    int getAvailable(){return available;};
    int getRented(){return rented;};
    node* getLeft(){return left;};
    node* getRight(){return right;};

    void setAvailable(int);
    void setRented(int);
    void setLeft(node*);
    void setRight(node*);

    void copyNode(node*);
};


#endif //PROJECT3_NODE_H
