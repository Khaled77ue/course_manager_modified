#ifndef COURSEMANAGER_H_
#define COURSEMANAGER_H_
//#include "AVL.h"

#include <assert.h>
#include "library2.h"
#include "AVL_rankTree.h"
#include <cstdio>
#define N 1000
#define C 5000

//#include <algorithm>



//#include "AVLTree.h"

class keyGen{
public:
    int class_id;
    int course_id;
    int streams;

    keyGen( int song_id=0,int artist_id=0,int streams=0):class_id(song_id),course_id(artist_id),
                                                         streams(streams){};
    ~keyGen()= default;
    keyGen(const keyGen& other)= default;
    bool operator<(const keyGen& e1) const{
        if(this->streams == e1.streams){
            if(this->course_id > e1.course_id){ //
                return true;
            } else{
                if(this->course_id == e1.course_id){
                    if(this->class_id > e1.class_id) { //
                        return true;
                    }
                }
            }
        }
        if(this->streams < e1.streams)  {
            return true;
        }
        return false;
    }

    bool operator>(const keyGen& e1) const {
        return e1 < *this;
    }

    bool operator==(const keyGen& e1) const{
        return this->streams == e1.streams && this->class_id == e1.class_id
               && this->course_id == e1.course_id;
    }

    bool operator<=(const keyGen& e1) const{
        if( *this == e1 || *this < e1){
            return true;
        }
    }

    bool operator>=(const keyGen& e1) const{
        return !( *this < e1);
    }

};

template <class T>
class  Vector {
public:
    //Vector(unsigned int size) : length(size) {};
    Vector(int size, const T& initial = T()) : length(size),current(0) {
        vec = new T[size];
        for (int i = 0; i < size; i++)
            vec[i] = initial;
    }
    Vector(const Vector<T>& v) {
        length = v.length;
        vec = new T[length];
        for (unsigned int i = 0; i < length; i++)
            vec[i] = v.vec[i];
        current=v.current;
    }

    ~Vector() {
        delete[] vec;
    }
    void  delete_vec() {
        delete[] vec;
    }
    void addVec(T& add){
        current++;
        vec[current]=add;
    }
    unsigned int size() const {
        return length;
    }
    T& operator[](unsigned int index) {
        return vec[index];
    }
private:
    int length;
    int current;
    T* vec;
};

class Empty {};

class  DynamicArray {
public:
    int length;
    int current;
    int* vec;

    DynamicArray(int size=N) : length(size), current(0) {
        vec = new int[size];
        for (int i = 0; i < size; i++) {
            vec[i] = 0;
        }
    }
    ~DynamicArray() {
        delete[] vec;
    }
    int& operator[](int index) {
        return vec[index];
    }
    void updateArr() {
        current++;
        if (current == length) {
            int* vec_new = new int[2 * length];
            for (int i = 0; i < length; i++) {
                vec_new[i] = vec[i];
            }
            delete[] vec;
            vec = vec_new;
            length = 2 * length;
        }
    }

};

class DoubleDirectionNode {
public:
    explicit DoubleDirectionNode(int CourseID) :courseID(CourseID),classeshist(N)
            ,listens(N),next(nullptr), previous(nullptr),current_num_of_classes(0){};
    ~DoubleDirectionNode() {clear();
    }
    void clear(){

        next= nullptr;
        previous= nullptr;
    }
	int courseID;
	DynamicArray classeshist;
	DynamicArray listens;
    DoubleDirectionNode* next;
	DoubleDirectionNode* previous;
	int current_num_of_classes;
    
};


class ClassesCount {
public:
    explicit ClassesCount(int artistID) :start(nullptr), end(nullptr),nodesAmount(1) {
        DoubleDirectionNode* tmp =   new DoubleDirectionNode(artistID);
        end = tmp;
        start = tmp;
    }

    ~ClassesCount() {
        deleteSongCount();
    }
    void deleteSongCount(){
        while (start != end) {
            DoubleDirectionNode* tmp = start;
            start = start->next;

            delete tmp;
        }
        if (start == end && start != nullptr && end != nullptr) {

            delete  end;
            start = nullptr;
            end = nullptr;
        }

    }
    void addNode(DoubleDirectionNode* current, unsigned int artistID) {
        DoubleDirectionNode* tmp = new DoubleDirectionNode(artistID);
        if (current->previous == nullptr) {
            tmp->next = current;
            tmp->previous = nullptr;
            current->previous = tmp;
            start = tmp;
        }
        else {
            DoubleDirectionNode* prev = current->previous;
            tmp->next = current;
            current->previous = tmp;
            prev->next = tmp;
            tmp->previous = prev;
        }
        nodesAmount++;
    }

    void addNode_copy(DoubleDirectionNode* current, DoubleDirectionNode* tmp2) {
        auto* tmp = new DoubleDirectionNode(tmp2->courseID);
        if (current->previous == nullptr) {
            tmp->next = current;
            tmp->previous = nullptr;
            current->previous = tmp;
            start = tmp;
        }
        else {
            DoubleDirectionNode* prev = current->previous;
            tmp->next = current;
            current->previous = tmp;
            prev->next = tmp;
            tmp->previous = prev;
        }
    }

    void removeNode(DoubleDirectionNode* current) {
        nodesAmount--;
        if (current->previous == nullptr && current->next == nullptr) {

            delete current;
            start= nullptr;
            end= nullptr;
            return;
        }
        if (current->previous == nullptr) {
            start = start->next;

            delete current;
            return;
        }
        if (current->next == nullptr) {
            end = current->previous;
            current->previous->next = nullptr;
            delete current;
            return;
        }
        current->next->previous = current->previous;
        current->previous->next = current->next;

        delete current;
    }

    DoubleDirectionNode* getStart() {
        return start;
    }
    DoubleDirectionNode* getEnd() {
        return end;
    }
    int getNodesAmount() {
        return nodesAmount;
    }

private:

    DoubleDirectionNode* start;
    DoubleDirectionNode* end;
    int nodesAmount;
};


class CourseManager {
public:
    CourseManager() : currSize(N),CoursesAmount(0), hashTable(N){


    };
    ~CourseManager(){

        for(int i=0;i<currSize;i++ ){
            if(hashTable.operator[](i)!= nullptr){
                delete  hashTable.operator[](i);}
        }

        RanksTree.cleara(RanksTree.Root);




    }



    void Remove_from_RanksTree(int artistID,int songID,int streams){
        auto* old=new keyGen(songID,artistID,streams);
        RanksTree.RemoveInTree(*old,0);
        delete old;

    }

    void add_to_RanksTree_with_newRank(int courseID,int ClassID,int streams,int oldstreams){
        auto* old=new keyGen(ClassID,courseID,oldstreams);
        RanksTree.RemoveInTree(*old,0);
        auto* tmp= new keyGen(ClassID,courseID,streams);
        keyGen tmp2=keyGen(ClassID,courseID,streams);
        RanksTree.putNodeInMyTree(1,tmp2,tmp,0);
        delete old;
    }

    void  Add_to_RanksTree_zero(int artistID,int songID){
        keyGen* tmp= new keyGen(songID,artistID,0);
        keyGen tmp2=keyGen(songID,artistID,0);

        RanksTree.putNodeInMyTree(1,tmp2,tmp,0);

    }

    void addCourse(const int artistID) {
        int place = (artistID) % (currSize);
        if (!hashTable.operator[](place)) {
            hashTable.operator[](place) = new ClassesCount(artistID);
        }
        else {
            hashTable.operator[](place)->addNode(hashTable.operator[](place)->getStart(), artistID);
        }
        CoursesAmount++;
        if (CoursesAmount == currSize) {
            double_size();
        }

    }

    void minimize_size() {
        Vector<ClassesCount*> new_1(int(0.5 * currSize));
        for (int i = 0; i < currSize; i++) {
            DoubleDirectionNode* tmp = hashTable.operator[](i)->getStart();
            while (tmp) {
                int place = (tmp->courseID) % (int(0.5 * currSize));
                new_1.operator[](place)->addNode_copy(new_1.operator[](place)->getStart(), tmp);
                tmp = tmp->next;
            }

        }
        hashTable.delete_vec();
        hashTable = new_1;
        currSize = int(0.5 * currSize);
    }

    void double_size() {
        Vector<ClassesCount*> new_1(2 * currSize);
        for (int i = 0; i < currSize; i++) {
            DoubleDirectionNode* tmp = hashTable.operator[](i)->getStart();
            while (tmp) {
                int place = (tmp->courseID) % (2 * currSize);
                new_1.operator[](place)->addNode_copy(new_1.operator[](place)->getStart(), tmp);
                tmp = tmp->next;
            }
        }
        hashTable.delete_vec();
        hashTable = new_1;

        currSize = 2 * currSize;
    }

    void  RemoveArtist_M(int CourseID) {
        int place = (CourseID) % (currSize);
        DoubleDirectionNode* tmp = hashTable.operator[](place)->getStart();
        while (tmp) {
            if (tmp->courseID == CourseID) break;
            else {
                tmp = tmp->next;
            }
        }
        if(tmp!= nullptr){
            Remove_artist_from_Rank_tree(CourseID);

            hashTable.operator[](place)->removeNode(tmp);
            if (hashTable.operator[](place)->getNodesAmount() == 0) {
                delete hashTable.operator[](place);
                hashTable.operator[](place) = nullptr;
            }
            CoursesAmount--;}


    }

  void  Remove_artist_from_Rank_tree(int CourseID){
      int place = (CourseID) % (currSize);
      DoubleDirectionNode* tmp = hashTable.operator[](place)->getStart();
      while (tmp) {
          if (tmp->courseID == CourseID) break;
          else {
              tmp = tmp->next;
          }
      }
      if(tmp!= nullptr){

   int num_of_classes= tmp->classeshist.current;
   for(int i=0;i<num_of_classes;i++){
       keyGen tmp2=keyGen(i,CourseID,tmp->listens[i]);
       RanksTree.RemoveInTree(tmp2,0);
   }
         }


    }

    DoubleDirectionNode* accessCourseData(int courseID) {
        int place = (courseID) % (currSize);
        if (!hashTable.operator[](place)) {
            return nullptr;
        }
        DoubleDirectionNode* tmp = hashTable.operator[](place)->getStart();

        while (tmp) {
            if (tmp->courseID == courseID) {
                break;
            }
            else {
                tmp = tmp->next;
            }
        }
        return tmp;
    }



    int getCurrSize() {
        return currSize;
    }

    AVL_TREE<keyGen,keyGen>& getRanksTree(){
        return RanksTree;
    };


private:


    int currSize;
    AVL_TREE<keyGen,keyGen> RanksTree;
    int CoursesAmount;
    Vector<ClassesCount*> hashTable;
};






#endif // !COURSEManager
