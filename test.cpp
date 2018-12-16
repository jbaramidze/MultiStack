#include "gtest/gtest.h"
#include <stdarg.h>
#include <vector>
#include "MyStackContainer.h"

#define INIT(size, splits) MyStackContainer *stacks = new MyStackContainer(size, splits); \
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



void stack_equals(MyStackContainer *stacks, int index, int size ...) {
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


TEST(MainTest, Errors) {
    INIT(4, 2);
    PUSH(0, 1);
    PUSH(0, 2);
    PUSH(0, 3);
    PUSH(1, 4);
    PUSH_FAIL(10, 4, ERR_INVALID_INDEX);
    POP_FAIL(11, ERR_INVALID_INDEX);
    TOP_FAIL(16, ERR_INVALID_INDEX);
    PUSH_FAIL(1, 5, ERR_OUT_OF_MEM);
}

TEST(MainTest, Simple_noshifts) {
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
}
 
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}