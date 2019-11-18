#ifndef PROJECT3_BINARYSEARCHTREE_H
#define PROJECT3_BINARYSEARCHTREE_H

#include "node.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <regex>

template<class T>
class binarySearchTree {
private:
    node* root = nullptr;
public:

    //recursive insert function
    void insert(node* i, node* r){
        //if tree is empty
        if(binarySearchTree::root==nullptr) {
            binarySearchTree::root = i;
            return;
        }
        else if (i->getTitle()>r->getTitle()){
            if(r->getRight())
                insert(i,r->getRight()); //recursive call
            else {
                r->setRight(i);
                return;
            }
        }
        else if (i->getTitle()<r->getTitle()){
            if(r->getLeft())
                insert(i,r->getLeft()); //recursive call
            else {
                r->setLeft(i);
                return;
            }
        }
    }

    void insert(node* i) { //helper method to simplify calling the insert function
        insert(i, binarySearchTree::root);
    }

    //recursive search function to return a pair of the node we search for and its parent
    std::pair<node*,node*> search(const std::string &title, node* s){
        //if tree is empty
        if(!s)
            return std::make_pair(nullptr, nullptr);
        //if the node is the root
        else if (s->getTitle()==title)
            return std::make_pair(nullptr, binarySearchTree::root);

        else if(s->getLeft() && s->getLeft()->getTitle()==title)
            return std::make_pair(s,s->getLeft());
        else if(s->getRight() && s->getRight()->getTitle()==title)
            return std::make_pair(s, s->getRight());

        if ( title < s->getTitle())
            search(title, s->getLeft()); //recursive call
        else if (title > s->getTitle())
            search(title, s->getRight()); //recursive call
        else {
            return std::make_pair(nullptr, nullptr);
        }
    }

    std::pair<node*,node*> search(const std::string &title){ //hepler method to simplify calling search function
        return search(title,binarySearchTree::root);
    }

    node* findMin(node* n) //find the min value of a given subtree
    {
        node* current = n;
        while (current->getLeft() != nullptr)
            current = current->getLeft();
        return current;
    }

    node* findMax(node* n) //finds the max value of a given subtree
    {
        node* current = n;
        while (current->getRight() != nullptr)
            current = current->getRight();
        return current;
    }

    void deleteNode(node* p, node* d) {
        //if we want to delete the root
        if(!p){
            //if no children
            if(!d->getLeft() && !d->getRight())
                binarySearchTree::root=nullptr;
            //Case 2: One child
            else if(!d->getRight()) { //if only a left child
                node* tempNode= new node();
                tempNode->copyNode(findMax(binarySearchTree::root->getLeft()));
                //delete the max node
                std::pair<node*,node*> tempPair = search(tempNode->getTitle());
                deleteNode(tempPair.first,tempPair.second);
                binarySearchTree::root->copyNode(tempNode);
            }
            else{ //if a Right child or multiple children
                node* tempNode= new node();
                tempNode->copyNode(findMin(binarySearchTree::root->getRight()));
                //delete the min node
                std::pair<node*,node*> tempPair = search(tempNode->getTitle());
                deleteNode(tempPair.first,tempPair.second);
                binarySearchTree::root->copyNode(tempNode);
            }
            return;
        }
        //node to delete has no children
        else if(!d->getLeft() && !d->getRight()) {
            if(d->getTitle()>p->getTitle()){
                p->setRight(nullptr);
            }
            else if(d->getTitle()<p->getTitle()){
                p->setLeft(nullptr);
            }
            delete d;
        }
        //has one child
        else if(!d->getLeft()) { //if only a right child
            if(d->getTitle()>p->getTitle()){
                p->setRight(d->getRight());
            }
            else if(d->getTitle()<p->getTitle()){
                p->setLeft(d->getRight());
            }
            delete d;
        }
        else if(!d->getRight()) { //if only a left child
            if(d->getTitle()>p->getTitle()){
                p->setRight(d->getLeft());
            }
            else if(d->getTitle()<p->getTitle()){
                p->setLeft(d->getLeft());
            }
            delete d;
        }
        //has multiple children
        else {
            node* tempNode= new node();
            tempNode->copyNode(findMin(d->getRight()));
            //delete the max node
            std::pair<node*,node*> tempPair = search(tempNode->getTitle());
            deleteNode(tempPair.first,tempPair.second);
            d->copyNode(tempNode);
        }
    }

    void deleteNode(const std::string &title){ //helper method to simplify calling the deleteNode function
        std::pair<node*,node*> tempPair = search(title);
        deleteNode(tempPair.first,tempPair.second);
    }

    int findLongest(node* p, int &len) {
        if(p) {
            if(p->getLeft()) {
                findLongest(p->getLeft(),len);
            }
            if(p->getTitle().length()>len)
                len=p->getTitle().length();
            if(p->getRight()) {
                findLongest(p->getRight(),len);
            }
        }
        return len;
    }

    void generateReport(node* p, std::ofstream &outData, int longestName){
        //inorder traversal
        //generate the report to the redbox_kiosk.txt file
        if(p) {
            if(p->getLeft()) {
                generateReport(p->getLeft(),outData,longestName);
            }
            outData << std::left << std::setw(longestName) << std::setfill(' ') << p->getTitle();
            outData << std::right << std::setw(5) << std::setfill(' ') << p->getAvailable();
            outData << std::right << std::setw(5) << std::setfill(' ') << p->getRented()<<std::endl;
            if(p->getRight()) {
                generateReport(p->getRight(),outData,longestName);
            }
        }
    }

    void generateReport(){ //helper method to simplify calling the generateReport function
        std::ofstream outData("redbox_kiosk.txt");
        int x=0;
        generateReport(binarySearchTree::root,outData,findLongest(binarySearchTree::root,x));
        outData.close();
    }

    void add(const std::string &title, const int &q){
        node* a = search(title).second;
        if(!a){
            a = new node(title,q,0);
            insert(a);
        }
        else
            a->setAvailable(a->getAvailable()+q);
    }

    void rent(const std::string &title){
        //remove one from quantity available
        //add one to quantity rented
        node* a = search(title).second;
        a->setAvailable(a->getAvailable()-1);
        a->setRented(a->getRented()+1);
    }

    void returnMovie(const std::string &title){
        //remove one from quantity rented
        //add one to quantity available
        node* a = search(title).second;
        a->setAvailable(a->getAvailable()+1);
        a->setRented(a->getRented()-1);
    }

    void remove(const std::string &title, const int &q){
        //remove q from quantity available
        //if no copies are available
        //and zero are rented then delete the node
        node* a = search(title).second;
        a->setAvailable(a->getAvailable()-q);
        if(a->getAvailable()<=0 && a->getRented()<=0)
            deleteNode(search(title).first,a);
    }

    binarySearchTree(){ //constructor
        //first create the BST based on inventory.dat
        std::ifstream inData("inventory.dat");
        std::smatch match;
        std::string line;
        node* i=nullptr;
        while(!inData.eof()){
            getline(inData,line);
            if(!line.empty()) {
                std::regex_search(line, match, std::regex(R"(\"(.+)\",(\d+),(\d+))"));
                i = new node(match[1], std::stoi(match[2]), std::stoi(match[3]));
                insert(i);
            }
        }
        inData.close();

        //now apply transactions from transaction.log
        //and add any errors to error.log

        std::cout<<std::endl;

        inData.open("transaction.log");
        std::ofstream outData("error.log");
        while(!inData.eof()){
            getline(inData,line);
            if(!line.empty()) {
                if(line.substr(0,3)=="add" && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\",(\d+))"))){
                    add(match[2],std::stoi(match[3]));
                }
                else if(line.substr(0,4)=="rent" && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\")")) && search(match[2]).second){
                    rent(match[2]);
                }
                else if(line.substr(0,6)=="return" && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\")")) && search(match[2]).second){
                    returnMovie(match[2]);
                }
                else if(line.substr(0,6)=="remove" && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\",(\d+))")) && search(match[2]).second){
                    remove(match[2],std::stoi(match[3]));
                }
                else{
                    outData<<line<<std::endl;
                }
            }
        }

        //generate report after transactions have been applied
        generateReport();
    }
};
#endif //PROJECT3_BINARYSEARCHTREE_H