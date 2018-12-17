#include <bits/stdc++.h>
#include "MultiStack.h"
using namespace std;


MultiStack::MultiStackItem::MultiStackItem(MultiStack *parent, int index, int start, int size, int capacity, bool growUpwards): 
        parent_(parent), index_(index), size_(size), start_(start), capacity_(capacity), growUpwards_(growUpwards)
{
}

bool MultiStack::MultiStackItem::growsUpwards()   
{ 
    return growUpwards_; 
}
bool MultiStack::MultiStackItem::growsDownwards() 
{ 
    return !growUpwards_; 
}

void MultiStack::MultiStackItem::incSize() 
{
    cout << "incSize() of #" << index_ << "\n";
    size_++;
}
void MultiStack::MultiStackItem::decSize() 
{
    cout << "decSize() of #" << index_ << "\n";
    size_--;
}
void MultiStack::MultiStackItem::incCap() 
{
    cout << "incCap() of #" << index_ << "\n";
    capacity_++;
}
void MultiStack::MultiStackItem::decCap() 
{
    cout << "decCap() of #" << index_ << "\n";
    capacity_--;
}
bool MultiStack::MultiStackItem::isEmpty() 
{
    return size_ == 0;
}

bool MultiStack::MultiStackItem::isFull()
{
  return size_ == capacity_;
}

int MultiStack::MultiStackItem::getStart() 
{
    return start_; 
}
int MultiStack::MultiStackItem::getSize()  
{
    return size_; 
}
int MultiStack::MultiStackItem::getCap()   
{
    return capacity_; 
}

int MultiStack::MultiStackItem::get(int i) 
{
    if (growsUpwards()) {
        return parent_ -> get(start_ + i);
    } else {
        return parent_ -> get(start_ - i);
    }
 }

void MultiStack::MultiStackItem::set(int i, int val)
{
    if (growsUpwards()) {
        parent_ -> set(start_ + i, val);
    } else {
        parent_ -> set(start_ - i, val);
    }
}

void MultiStack::MultiStackItem::push(int val) 
{
    assert(size_ < capacity_);
    set(size_, val);
    incSize();
}

int MultiStack::MultiStackItem::top() 
{
    assert(size_ > 0);
    return get(size_ - 1);
}

int MultiStack::MultiStackItem::pop() 
{
    assert(size_ > 0);
    int r = top();
    decSize();
    return r;
}

void MultiStack::MultiStackItem::shiftUp() 
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

    start_ = (start_ + 1) % (parent_ -> totalSize());
}

void MultiStack::dump()
{
    cout << "###########################################################\n";
    cout << "###########################################################\n";
    for (int i = 0; i < numStacks_; i++) {
        cout << "Dumping stack #" << i << ":\n";
        MultiStackItem *stack = stacks_[i];
        stack -> dump();
    }
    cout << "###########################################################\n";
    cout << "###########################################################\n";
}

void MultiStack::MultiStackItem::dump() 
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

bool MultiStack::isValidStackIndex(int index)
{
    return index >= 0 && index < numStacks_;
}

MultiStack::MultiStack(int totalSize, int numStacks)
{
    data_ = new int[totalSize];
    stacks_ = new MultiStackItem*[numStacks];

    int size_each = totalSize/numStacks;

    for (int i = 0; i < numStacks; i++)
    {
        int begin = (i % 2 == 0) ? i * size_each : ((i + 1) * size_each - 1);
        
        int size = size_each;
        if (i == numStacks - 1)
        {
            size += (totalSize % numStacks);
            if (i % 2 == 1) begin = totalSize - 1;
        }
        stacks_[i] = new MultiStackItem(this, i, begin, 0, size, i % 2 == 0);
    }

    numStacks_ = numStacks;
    size_ = totalSize;
}

void MultiStack::set(int offset, int val) 
{
    int address = offset % size_;
    if (address < 0) address += size_;

    data_[address] = val;
}

int MultiStack::get(int offset) 
{
    int address = offset % size_;
    if (address < 0) address += size_;

    return data_[address];
}

void MultiStack::push(int index, int val, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return; }
    MultiStackItem *stack = stacks_[index];

    if (stack -> isFull()) {
        if (!expand(index, error)) return;    
    }

    stack -> push(val);
}

int MultiStack::top(int index, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MultiStackItem *stack = stacks_[index];

    return stack -> top();
}

int MultiStack::pop(int index, int &error) 
{
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MultiStackItem *stack = stacks_[index];

    return stack -> pop();
}


int MultiStack::size(int index, int &error) {
    if (!isValidStackIndex(index)) { error = ERR_INVALID_INDEX; return 0; }
    MultiStackItem *stack = stacks_[index];

    return stack -> getSize();
}

bool MultiStack::allFull()
{
    int size = 0;
    for (int i = 0; i < numStacks_; i++) size += stacks_[i] -> getSize();

    return size == size_;
}


int MultiStack::totalSize()
{
    return size_;
}

void MultiStack::shiftUp(int index)
{
    cout << "Shifting up index " << index << ".\n";
    assert(isValidStackIndex(index));
    assert(!allFull());
    assert(stacks_[index] -> growsUpwards());

    // First, check if only shifting us is enough.
    MultiStackItem *stack = stacks_[index];
    if (!stack -> isFull()) {
        stack -> shiftUp();
        stack -> decCap();
        return;
    }

    // If the above stack has some space, shrink it 
    MultiStackItem *next = stacks_[getNextIndex(index)];
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

bool MultiStack::expand(int index, int &error)
{
    cout << "Expanding index " << index << ".\n";
    assert(isValidStackIndex(index));
    if (allFull()) { error = ERR_OUT_OF_MEM; return false; }

    MultiStackItem *stack = stacks_[index];
    MultiStackItem *pair = stack -> growsUpwards() ? stacks_[getNextIndex(index)] : stacks_[getPrevIndex(index)];

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

int MultiStack::getNextIndex(int index) 
{ 
    return (index + 1) % numStacks_;
}

int MultiStack::getPrevIndex(int index)
{ 
    return (index + numStacks_ - 1) % numStacks_;
}