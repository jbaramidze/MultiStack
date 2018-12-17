#include "gtest/gtest.h"
#include <stdarg.h>
#include <vector>
#include "MultiStack.h"

#define INIT(size, splits) MultiStack *stacks = new MultiStack(size, splits); \
                           int error = -1;                                                \
                           int res = -1;

#define PUSH(i, val) stacks -> push(i, val, error);   \
                     ASSERT_EQ(error, -1);


#define POP(i) res = stacks -> pop(i, error); \
               ASSERT_EQ(error, -1);

#define TOP(i) res = stacks -> top(i, error); \
               ASSERT_EQ(error, -1);

#define PUSH_FAIL(i, val, err) stacks -> push(i, val, error);   \
                               ASSERT_EQ(error, err);

#define POP_FAIL(i, err) res = stacks -> pop(i, error); \
                               ASSERT_EQ(error, err);

#define TOP_FAIL(i, err) res = stacks -> top(i, error); \
                               ASSERT_EQ(error, err);



void stack_equals(MultiStack *stacks, int index, int size ...) {
	va_list ap;
	int error = -1;
	int res = -1;
    std::vector<int> popped;
    va_start(ap, size);
    ASSERT_EQ(size, stacks -> size(index, error));
    ASSERT_EQ(error, -1);

    for (int i = 0; i < size; i++) {
    	POP(index);
    	popped.push_back(res);
    	int expected = va_arg(ap, int);
    	ASSERT_EQ(res, expected);
        ASSERT_EQ(error, -1);
    }

    ASSERT_EQ(0, stacks -> size(index, error));

    // Restore the stack.
    for (int i = size - 1; i >= 0; i--) {
    	PUSH(index, popped[i]);
    }
    va_end(ap);
}


TEST(MainTest, errors) {
    INIT(4, 2);
    PUSH(0, 1);
    PUSH(0, 2);
    PUSH(0, 3);
    PUSH(1, 4);
    PUSH_FAIL(10, 4, ERR_INVALID_INDEX);
    POP_FAIL(11, ERR_INVALID_INDEX);
    TOP_FAIL(16, ERR_INVALID_INDEX);
    PUSH_FAIL(1, 5, ERR_OUT_OF_MEM);
    PUSH_FAIL(0, 5, ERR_OUT_OF_MEM);
}

TEST(MainTest, noShifts) {
    INIT(4, 2);

    PUSH(0, 1);
    PUSH(0, 2);
    PUSH(1, 3);
    PUSH(0, 4);
    stack_equals(stacks, 0, 3, 4, 2, 1);
    stack_equals(stacks, 1, 1, 3);

    POP(0);
    PUSH(1, 8);
    stack_equals(stacks, 0, 2, 2, 1);
    stack_equals(stacks, 1, 2, 8, 3);

    POP(0);
    POP(0);
    PUSH(1, 11); 
    PUSH(1, 12);
    stack_equals(stacks, 0, 0);
    stack_equals(stacks, 1, 4, 12, 11, 8, 3);
}

TEST(MainTest, shiftNext) {
    INIT(8, 4);

    PUSH(2, 20);
    PUSH(2, 21);
    PUSH(1, 11);
    PUSH(0, 1);
    PUSH(0, 2);
    PUSH(0, 3);
    PUSH(0, 4);
    PUSH(0, 5);

    stack_equals(stacks, 2, 2, 21, 20);
    stack_equals(stacks, 0, 5, 5, 4, 3, 2, 1);
    stack_equals(stacks, 1, 1, 11);
}

TEST(MainTest, OverflowEven) {
    INIT(9, 3);
    PUSH(0, 1);

    PUSH(2, 21);
    PUSH(2, 22);
    PUSH(2, 23);
    PUSH(2, 24);
    PUSH(2, 25);

    stack_equals(stacks, 0, 1, 1);
    stack_equals(stacks, 2, 5, 25, 24, 23, 22, 21);
}

TEST(MainTest, OverflowEven2) {
    INIT(10, 5);
    PUSH(0, 1);

    PUSH(4, 41);
    PUSH(4, 42);
    PUSH(4, 43);

    PUSH(2, 21);
    PUSH(2, 22);
    PUSH(2, 23);
    PUSH(2, 24);
    PUSH(2, 25);
    PUSH(2, 26);

    stack_equals(stacks, 0, 1, 1);
    stack_equals(stacks, 2, 6, 26, 25, 24, 23, 22, 21);
    stack_equals(stacks, 4, 3, 43, 42, 41);
}

TEST(MainTest, OverflowOdd) {
    INIT(14, 6);

    PUSH(4, 41);
    PUSH(5, 51);

    PUSH(2, 21);
    PUSH(2, 22);
    PUSH(2, 23);
    PUSH(2, 24);
    PUSH(2, 25);
    PUSH(2, 26);
    PUSH(2, 27);
    PUSH(2, 28);
    PUSH(2, 29);
    PUSH(2, 210);
    PUSH(2, 211);

    stack_equals(stacks, 2, 11, 211, 210, 29, 28, 27, 26, 25, 24, 23, 22, 21);
    stack_equals(stacks, 4, 1, 41);
    stack_equals(stacks, 5, 1, 51);
}

TEST(MainTest, UnderflowEvenOdd) {
    INIT(10, 5);

    PUSH(4, 41);

    PUSH(3, 21);
    PUSH(3, 22);
    PUSH(3, 23);
    PUSH(3, 24);
    PUSH(3, 25);
    PUSH(3, 26);
    PUSH(3, 27);
    PUSH(3, 28);
    PUSH(3, 29);

    stack_equals(stacks, 3, 9, 29, 28, 27, 26, 25, 24, 23, 22, 21);
    stack_equals(stacks, 4, 1, 41);
}
 
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
