//
//  main.cpp
//  extendible_hashing_cpp
//
//  Created by Harsh Meena on 16/10/22.
//
//****LIBRARIES****//
 
#include <iostream>
#include <cctype>
#include <cstring>
#include <set>
#include <climits>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <iomanip>
#include <ctype.h>
#include <stdio.h>
#include <utility>
using namespace std;

#define HAANJI 55555

class Bucket
{
    const long unsigned int size;
    int depth;
    list<pair<const int, string>> values;
    bool updateID(const int ID, const string value);
    string getValueFromID(const int ID);

  public:
    Bucket(const int depth, const int size);
    bool insert(const int ID, const string value);
    int getDepth()
    {
        return this->depth;
    };
    list<pair<const int, string>> getValues()
    {
        return this->values;
    };
    int findID(const int ID);
    bool remove(const int ID);
};

Bucket::Bucket(const int depth, const int size) : size(size), depth(depth){}

int Bucket::findID(const int ID) {
    int counter = 0;
    for (pair<const int, const string> p : this->values) {
        if (p.first == ID)
            return counter;
        counter++;
    }
    return -1;
}

string Bucket::getValueFromID(const int ID) {
    for (pair<const int, const string> p : this->values) {
        if (p.first == ID)
            return p.second;
    }
    return "\0";
}

bool Bucket::updateID(const int ID, const string value) {
    for (pair<const int, string> &p : this->values) {
        if (p.first == ID) {
            p.second = value;
            return true;
        }
    }

    return false;
}

bool Bucket::insert(const int ID, const string value) {

    if (findID(ID) >= 0) {
        this->updateID(ID, value);
        return true;
    }

    if (this->values.size() < size) {
        this->values.emplace_back(make_pair(ID, value));
        return true;
    } else
        return false;
}

bool Bucket::remove(const int ID) {
    if (findID(ID) >= 0) {
        this->values.remove({ID, getValueFromID(ID)});
        return true;
    } else {
        return false;
    }
}

class Directory {
    const long unsigned int bucket_size;
    int global_depth;
    vector<Bucket *> buckets;
    int hashing_func(const int n) { return n % HAANJI; };
    void grow();
    void split(int bucket_no);
    int countBuckets() { return 1 << global_depth; };
    int getPosFromKey(const int key) {
        return this->hashing_func(key) % this->countBuckets();
    }

  public:
    Directory(const int depth, const int bucket_size);
    void insert(const int key, const string value);
    void remove(const int key);

    void print();
};

Directory::Directory(const int depth, const int bucket_size)
    : bucket_size(bucket_size), global_depth(depth) {
    this->buckets = vector<Bucket *>(this->countBuckets(),
                                     new Bucket(depth, bucket_size));
}

void Directory::split(const int bucket_no) {
// get the bucket pos pointing to the same
// its always 0xxx and 1xxx
// cerr << "(d_split) " << bucket_no << '\n';
    const int b_pos1 = bucket_no % (this->countBuckets()/ 2);
    const int b_pos2 = b_pos1 + (this->countBuckets() / 2);

    Bucket *b = this->buckets[bucket_no];

    this->buckets[b_pos1] = new Bucket(b->getDepth() + 1, this->bucket_size);
    this->buckets[b_pos2] = new Bucket(b->getDepth() + 1, this->bucket_size);

    for (pair<const int, const string> p : b->getValues()) {
        this->insert(p.first, p.second);
    }

    delete b;
// cerr << "(d_split) done" << '\n';
}

void Directory::grow() {
    cerr << "(d_grow) to = " << this->global_depth + 1 << '\n';
    const vector<Bucket *> temp = this->buckets;
    this->buckets.insert(buckets.end(), temp.begin(), temp.end());
    this->global_depth++;
}

void Directory::insert(const int key, const string value) {
    cerr << "(d_insert) key = " << key << " ";

    const int pos = this->getPosFromKey(key);

    cerr << "pos = " << pos << '\n';

    if (!this->buckets[pos]->insert(key, value)) {
        // split the bucket or grow the directory
        if (this->buckets[pos]->getDepth() < this->global_depth) {
            // split the bucket
            this->split(pos);
        } else {
            // grow
            this->grow();
        }

        this->insert(key, value);
    }
}

void Directory::remove(const int key) {
    const int pos = this->getPosFromKey(key);
    if(this->buckets[pos]->remove(key)) {
        cerr << "(d_remove) successfully removed\n";
    }
}

int main() {
    cout << "EXTENDIBLE_HASHING\n";
   
    Directory d(1, 1);
    d.insert(1, "HOLA");
    d.insert(1, "HI");
    d.insert(7, "HAANJI");
    d.insert(3, "KAISE HO");
    d.remove(3);
    cout << endl;
    return 0;
}
