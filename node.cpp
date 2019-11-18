#include "node.h"

node::node(){
    title="";
    available=0;
    rented=0;
    left=nullptr;
    right=nullptr;
}

node::node(std::string t,int q, int r){
    title=t;
    available=q;
    rented=r;
    left=nullptr;
    right=nullptr;
}

void node::setAvailable(int a) { available = a; }

void node::setRented(int r) { rented = r; }

void node::setLeft(node *l) { left = l; }

void node::setRight(node *r) { right = r; }

void node::copyNode(node * c){
    title=c->getTitle();
    available=c->getAvailable();
    rented= c->getRented();
}