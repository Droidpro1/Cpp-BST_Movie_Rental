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
    node* search(const std::string &title, node* s){
        //if tree is empty
        if(!s)
            return nullptr;
        //if the node is the root
        if (s->getTitle()==title)
            return s;
        if ( title < s->getTitle())
            search(title, s->getLeft()); //recursive call
        else if (title > s->getTitle())
            search(title, s->getRight()); //recursive call
    }

    node* search(const std::string &title){ //hepler method to simplify calling search function
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

    node* deleteNode(node* toDelete, node* iter) {
        if (!iter) return iter;
        if (toDelete->getTitle() < iter->getTitle())
            iter->setLeft(deleteNode(toDelete, iter->getLeft()));
        else if (toDelete->getTitle() > iter->getTitle())
            iter->setRight(deleteNode(toDelete, iter->getRight()));
        else{ //the keys are equal, so this is the one to delete
            // node with only one or no children
            if (!iter->getLeft()){
                node* temp = iter->getRight();
                delete(iter);
                return temp;
            }
            else if (!iter->getRight()){
                struct node *temp = iter->getLeft();
                delete(iter);
                return temp;
            }
            // if there are 2 children, find inorder successor
            node* temp = findMin(iter->getRight());
            // Copy its content to the current node
            iter->copyNode(temp);
            // then delete the inorder successor
            iter->setRight(deleteNode(temp, iter->getRight()));
        }
    }

    node* deleteNode(const std::string &title){ //helper method to simplify calling the deleteNode function
        node* toDelete = search(title);
        deleteNode(toDelete,root);
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
        std::ofstream outData("redbox_kiosk.txt", std::ofstream::out | std::ofstream::trunc);
        int x=0;
        generateReport(binarySearchTree::root,outData,findLongest(binarySearchTree::root,x));
        outData.close();
    }

    void add(const std::string &title, const int &q){
        node* a = search(title);
        if(!a){
            a = new node(title,q,0);
            insert(a);
        }
        else
            a->setAvailable(a->getAvailable()+q);
    }

    void rent(const std::string &title, const std::string& line){
        //remove one from quantity available
        //add one to quantity rented
        node* a = search(title);
        //if the movie doesnt exist or there arent any available to rent
        //the command is invalid
        if(!a || a->getAvailable()==0)
            invalidLine(line);
        else{
            a->setAvailable(a->getAvailable()-1);
            a->setRented(a->getRented()+1);
        }
    }

    void returnMovie(const std::string &title, const std::string& line){
        //remove one from quantity rented
        //add one to quantity available
        node* a = search(title);
        if(a->getRented()==0)
            invalidLine(line);
        else{
            a->setAvailable(a->getAvailable()+1);
            a->setRented(a->getRented()-1);
        }
    }

    void remove(const std::string &title, const int &q, const std::string &line){
        //remove q from quantity available
        //if no copies are available
        //and zero are rented then delete the node
        node* a = search(title);
        if(!a || a->getAvailable()-q<0){
            invalidLine(line);
        }
        else{
            a->setAvailable(a->getAvailable()-q);
            if(a->getAvailable()<=0 && a->getRented()<=0)
                deleteNode(a, root);
        }
    }

    void invalidLine(const std::string& line){
        std::ofstream outData("error.log", std::ofstream::out | std::ofstream::app);
        outData<<line<<std::endl;
        outData.close();
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

        //open and clear the contents of the error log if it already exists
        std::ofstream outData("error.log");
        outData.clear();
        outData.close();

        inData.open("transaction.log");
        while(!inData.eof()){
            getline(inData,line);
            if(!line.empty()) {
                if(line.substr(0,4)=="add " && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\",(\d+))"))){
                    add(match[2],std::stoi(match[3]));
                }
                else if(line.substr(0,5)=="rent " && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\")")) && search(match[2])){
                    rent(match[2], line);
                }
                else if(line.substr(0,7)=="return " && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\")")) && search(match[2])){
                    returnMovie(match[2], line);
                }
                else if(line.substr(0,7)=="remove " && std::regex_search(line, match, std::regex(R"((.+)\"(.+)\",(\d+))")) && search(match[2])){
                    remove(match[2],std::stoi(match[3]), line);
                }
                else{ //if the "keyword" doesnt match any of the commands, the line is an error
                    invalidLine(line);
                }
            }
        }

        //generate report after transactions have been applied
        generateReport();
    }
};
#endif //PROJECT3_BINARYSEARCHTREE_H