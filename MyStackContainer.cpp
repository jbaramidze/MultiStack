#include <bits/stdc++.h>
#include "MyStackContainer.h"
using namespace std;


MyStackContainer::MyStack::MyStack(MyStackContainer *parent, int index, int start, int size, int capacity, bool growUpwards): 
        parent_(parent), index_(index), size_(size), start_(start), capacity_(capacity), growUpwards_(growUpwards)
{
}

bool MyStackContainer::MyStack::growsUpwards()   
{ 
    return growUpwards_; 
}
bool MyStackContainer::MyStack::growsDownwards() 
{ 
    return !growUpwards_; 
}

void MyStackContainer::MyStack::incSize() 
{
    cout << "incSize() of #" << index_ << "\n";
    size_++;
}
void MyStackContainer::MyStack::decSize() 
{
    cout << "decSize() of #" << index_ << "\n";
    size_--;
}
void MyStackContainer::MyStack::incCap() 
{
    cout << "incCap() of #" << index_ << "\n";
    capacity_++;
}
void MyStackContainer::MyStack::decCap() 
{
    cout << "decCap() of #" << index_ << "\n";
    capacity_--;
}
bool MyStackContainer::MyStack::isEmpty() 
{
    return size_ == 0;
}

bool MyStackContainer::MyStack::isFull()
{
  return size_ == capacity_;
}

int MyStackContainer::MyStack::getStart() 
{
    return start_; 
}
int MyStackContainer::MyStack::getSize()  
{
    return size_; 
}
int MyStackContainer::MyStack::getCap()   
{
    return capacity_; 
}

int MyStackContainer::MyStack::get(int i) 
{
    if (growsUpwards()) {
        return parent_ -> get(start_ + i);
    } else {
        return parent_ -> get(start_ - i);
    }
 }

void MyStackContainer::MyStack::set(int i, int val)
{
    if (growsUpwards()) {
        parent_ -> set(start_ + i, val);
    } else {
        parent_ -> set(start_ - i, val);
    }
}

void MyStackContainer::MyStack::push(int val) 
{
    assert(size_ < capacity_);
    set(size_, val);
    incSize();
}

int MyStackContainer::MyStack::top() 
{
    assert(size_ > 0);
    return get(size_ - 1);
}

int MyStackContainer::MyStack::pop() 
{
    assert(size_ > 0);
    int r = top();
    decSize();
    return r;
}

void MyStackContainer::MyStack::shiftUp() 
{
    cout << "shiftUp() of #" << index_ << "\n";

    if (growsUpwards()) {
        for (int i = size_ - 1; i>=0; i--) {
             set(i + 1, get(i));
        }
    } else {
        for (int i = 0; i < size_; i++) {
             set(i - 1, get(i));
        }
    }

    start_++;
}

void MyStackContainer::dump()
{
    cout << "###########################################################\n";
    cout << "###########################################################\n";
    for (int i = 0; i < numStacks_; i++) {
        cout << "Dumping stack #" << i << ":\n";
        MyStack *stack = stacks_[i];
        stack -> dump();
    }
    cout << "###########################################################\n";
    cout << "###########################################################\n";
}

void MyStackContainer::MyStack::dump() 
{
    cout << "Size: " << size_ << " Start: " << start_ << " Capacity: " << capacity_ << (growUpwards_ ? " Upwards\n" : " Downwards\n");
    cout << "Elements:";
    if (growsUpwards()) {
        for (int i = 0; i < size_; i++) cout << parent_ -> get(start_ + i) << " ";
    } else {
        for (int i = 0; i < size_; i++) cout << parent_ -> get(start_ - i) << " ";
    }
    cout << "\n";
}

bool MyStackContainer::isValidStackIndex(int index)
{
    return index >= 0 && index < numStacks_;
}

MyStackContainer::MyStackContainer(int totalSize, int numStacks)
{
    data_ = new int[totalSize];
    stacks_ = new MyStack*[numStacks];

    int size_each = totalSize/numStacks;

    for (int i = 0; i < numStacks; i++)
    {
        int begin = (i % 2 == 0) ? i * size_each : ((i + 1) * size_each - 1);
        
        int size = size_each;
        if (i == numStacks - 1) size += (totalSize % numStacks);

        stacks_[i] = new MyStack(this, i, begin, 0, size, i % 2 == 0);
    }

    numStacks_ = numStacks;
    size_ = totalSize;
}

void MyStackContainer::set(int offset, int val) 
{
    //cout << "Setting " << val << " at " << offset << ".\n";
    data_[offset % size_] = val;
}

int MyStackContainer::get(int offset) 
{
    //cout << "Gettting from " << offset << ".\n";
    return data_[offset % size_];
}

void MyStackContainer::push(int index, int val, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return; }
    MyStack *stack = stacks_[index];

    if (stack -> isFull()) {
        if (!expand(index, error)) return;    
    }

    stack -> push(val);
}

int MyStackContainer::top(int index, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MyStack *stack = stacks_[index];

    return stack -> top();
}

int MyStackContainer::pop(int index, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MyStack *stack = stacks_[index];

    return stack -> pop();
}


int MyStackContainer::size(int index, int &error) {
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MyStack *stack = stacks_[index];

    return stack -> getSize();
}

bool MyStackContainer::allFull()
{
    int size = 0;
    for (int i = 0; i < numStacks_; i++) size += stacks_[i] -> getSize();

    return size == size_;
}

void MyStackContainer::shiftUp(int index)
{
    cout << "Shifting up index " << index << ".\n";
    assert(isValidStackIndex(index));
    assert(!allFull());
    assert(stacks_[index] -> growsUpwards());

    // First, check if only shifting us is enough.
    MyStack *stack = stacks_[index];
    if (!stack -> isFull()) {
        stack -> shiftUp();
        stack -> decCap();
        return;
    }

    // If the above stack has some space, shrink it 
    MyStack *next = stacks_[getNextIndex(index)];
    if (next -> growsDownwards() && !next -> isFull()) {
        next -> decCap();
        stack -> shiftUp();
        return;
    }

    // If none of those worked, ask the same to next group.
    if (next -> growsDownwards())
    {
        shiftUp(getNextIndex(getNextIndex(index)));
        next -> shiftUp();
        stack -> shiftUp();
    } else {
        // Case of overflow.
        shiftUp(getNextIndex(index));
        stack -> shiftUp();
    }


}

bool MyStackContainer::expand(int index, int &error)
{
    cout << "Expanding index " << index << ".\n";
    assert(isValidStackIndex(index));
    if (allFull()) { error = ERR_OUT_OF_MEM; return false; }

    MyStack *stack = stacks_[index];
    MyStack *pair = stack -> growsUpwards() ? stacks_[getNextIndex(index)] : stacks_[getPrevIndex(index)];

    if (stack -> growsUpwards() && pair -> growsDownwards()) {
        if (pair -> isFull()) {
            shiftUp(getNextIndex(getNextIndex(index)));
            pair -> shiftUp();
            stack -> incCap();
        } else {
            pair -> decCap();
            stack -> incCap();
        }
    } else if (stack -> growsDownwards() && pair -> growsUpwards()) {
        if (pair -> isFull()) {
            shiftUp(getNextIndex(index));
            stack -> shiftUp();
            stack -> incCap();
        } else {
            pair -> decCap();
            stack -> incCap();
        }
    } else if (stack -> growsUpwards() && pair -> growsUpwards()) {
        // This is the case when stack is the last one on top and we have odd stacks.
        if (pair -> isFull()) {
            shiftUp(getNextIndex(index));
            stack -> incCap();
        } else {
            shiftUp(getNextIndex(index));
            stack -> incCap();
        }
    } else {
        assert(false);
    }

    return true;
}

int MyStackContainer::getNextIndex(int index) 
{ 
    return (index + 1) % numStacks_;
}

int MyStackContainer::getPrevIndex(int index)
{ 
    return (index + numStacks_ - 1) % numStacks_;
}