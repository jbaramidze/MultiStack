#include <assert.h>

/*
  Stack Layout:
  /------------/
  /            /
  /     /\     /
  /     ||     /
  /  Stack #4  /
  /------------/
  /  Stack #3  /
  /     ||     /
  /     \/     /
  /            /
  /------------/
  /            /
  /     /\     /
  /     ||     /
  /  Stack #2  /
  /------------/
  /  Stack #1  /
  /     ||     /
  /     \/     /
  /            /
  /------------/
  /            /
  /     /\     /
  /     ||     /
  /  Stack #0  /
  /------------/
*/

#define ERR_INVALID_INDEX 1
#define ERR_OUT_OF_MEM    2

class MyStackContainer {

    class MyStack {

        public:
        MyStack(MyStackContainer *parent, int index, int start, int size, int capacity, bool growUpwards);

        bool growsUpwards();
        bool growsDownwards();

        void incSize();
        void decSize();
        void decCap();
        void incCap();
        bool isEmpty();
        bool isFull();

        int getStart();
        int getSize();
        int getCap(); 

        int get(int i);
        void set(int i, int val);

        void push(int val);
        int top();
        int pop();
        void shiftUp();

        void dump();

        private:
        int size_;
        int start_;
        int capacity_;
        bool growUpwards_;
        MyStackContainer *parent_;

        // Only for debugging.
        int index_;
    };

    bool isValidStackIndex(int index);

    public:
    MyStackContainer(int size, int numStacks);

    // make private & friend.
    void set(int offset, int val);
    int get(int offset);

    void push(int index, int val, int &error);
    int top(int index, int &error);
    int pop(int index, int &error);
    int size(int index, int &error);

    void dump();

    private:

    bool allFull();
    void shiftUp(int index);
    bool expand(int index, int &error);

    int getNextIndex(int index);
    int getPrevIndex(int index);

    int *data_;
    int size_;

    MyStack **stacks_;
    int numStacks_;
};