#include <stdio.h>
#include <stdlib.h>
#include <sysinfoapi.h>

#define MAX_STACK_SIZE 268437500
//2048 MiB

union Data {
    int intData;
    char charData;
    float floatData;
};


struct StackNode {
    union Data stackData;
    int type;
    /*types:
 * 1 - int
 * 2 - char
 * 3 - float
 * */
    struct StackNode *next;  //pointer to next node
};

struct Stack {
    struct StackNode *top;  //top element
    int size;// current size
    int max_size;//size limit
} ;


struct Stack *createStack(int max_size) {
    struct Stack *newStack = (struct Stack *)malloc(sizeof(struct Stack));
    if (newStack == NULL) {
        printf("Memory allocation failed.\n");
    } else {
        newStack->top = NULL;
        newStack->size = 0;
        newStack->max_size = max_size;
        return newStack;
    }
}

int isEmpty(struct Stack *stack) {
    return stack->top == NULL;
}

int isFull(struct Stack *stack) {
    return (stack->max_size > 0) && (stack->size == stack->max_size);
}

void intPush(struct Stack *stack, int intData){
    if (isFull(stack)) {
        printf("Stack is full\n");
    } else {
        struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
        } else {
            newNode->type = 1;
            newNode->stackData.intData = intData;
            newNode->next = stack->top;
            stack->top = newNode;
            stack->size++;
            }
        }
    }

void charPush(struct Stack *stack, char charData){
    if (isFull(stack)) {
        printf("Stack is full\n");
    } else {
        struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
        } else {
            newNode->type = 2;
            newNode->stackData.charData = charData;
            newNode->next = stack->top;
            stack->top = newNode;
            stack->size++;
        }
    }
}

void floatPush(struct Stack *stack, float floatData){
    if (isFull(stack)) {
        printf("Stack is full\n");
    } else {
        struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
        } else {
            newNode->type = 3;
            newNode->stackData.floatData = floatData;
            newNode->next = stack->top;
            stack->top = newNode;
            stack->size++;
        }
    }
}

int popInt(struct Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow. Cannot pop from an empty stack.\n");
        exit(1);
    }
    struct StackNode *topNode = stack->top;
    stack->top = topNode->next;
    int data = topNode->stackData.intData;
    free(topNode);
    stack->size--;
    return data;
}

char popChar(struct Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow. Cannot pop from an empty stack.\n");
        exit(1);
    }
    struct StackNode *topNode = stack->top;
    stack->top = topNode->next;
    char data = topNode->stackData.charData;
    free(topNode);
    stack->size--;
    return data;
}

float popFloat(struct Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow. Cannot pop from an empty stack.\n");
        exit(1);
    }
    struct StackNode *topNode = stack->top;
    stack->top = topNode->next;
    float data = topNode->stackData.floatData;
    free(topNode);
    stack->size--;
    return data;
}

union Data peek(struct Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack is empty. Cannot peek.\n");
    } else {
        return stack->top->stackData;
    }
}

void freeStack(struct Stack *stack) {
    while (!isEmpty(stack)) {
        switch (stack->top->type) {
            case 1:
                popInt(stack);
                break;
            case 2:
                popChar(stack);
                break;
            case 3:
                popFloat(stack);
                break;
        }
    }
    free(stack);
}

void displayStack(struct Stack *stack) {
    struct StackNode *current = stack->top;
    printf("\nStack elements: \n");
    if (current == NULL){
        printf("(empty stack)");
    }
    while (current != NULL) {
        switch (current->type) {
            case 1:
                printf("%d ", current->stackData.intData);
                current = current->next;
                break;
            case 2:
                printf("%c ", current->stackData.charData);
                current = current->next;
                break;
            case 3:
                printf("%f ", current->stackData.floatData);
                current = current->next;
                break;
        }
    }
    printf("\n");
    printf("Total Stack Size: %d\n", stack->max_size);
    printf("Used: %d\n", stack->size);
    printf("Free: %d\n", stack->max_size - stack->size);
}


int main() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status); //https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-memorystatusex
    GlobalMemoryStatusEx(&status);
    unsigned long long available_ram = status.ullAvailPhys / 2; //half of the available RAM in bytes

    struct Stack *stack = createStack(available_ram / 8); // one stack node occupies 8 bytes
    int choice;
    int type;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. isEmpty\n");
        printf("5. isFull\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Choose data type:\n");
                printf("1 : Integer\n");
                printf("2 : Char\n");
                printf("3 : Float\n");
                printf("Enter your choice: ");
                fflush(stdin);
                scanf("%d", &type);
                switch (type) {
                    case 1:
                        printf("Enter the int to push: ");
                        int ivalue;
                        fflush(stdin);
                        scanf("%d", &ivalue);
                        intPush(stack, ivalue);
                        break;
                    case 2:
                        printf("Enter the char to push: ");
                        fflush(stdin);
                        char cvalue;
                        scanf("%c", &cvalue);
                        charPush(stack, cvalue);
                        break;
                    case 3:
                        printf("Enter the float to push: ");
                        float fvalue;
                        fflush(stdin);
                        scanf("%f", &fvalue);
                        floatPush(stack, fvalue);
                        break;
                    default:
                        printf("No such type");
                        break;
                }
                displayStack(stack);
                break;
            case 2:
                if (!isEmpty(stack)) {
                    int ipopped;
                    char cpopped;
                    float fpopped;
                    switch (stack->top->type) {
                        case 1:
                            ipopped = popInt(stack);
                            printf("Popped element: %d \n", ipopped);
                            break;
                        case 2:
                            cpopped = popChar(stack);
                            printf("Popped element: %c \n", cpopped);
                            break;
                        case 3:
                            fpopped = popFloat(stack);
                            printf("Popped element: %f \n", fpopped);
                            break;
                    }
                } else {
                    printf("Stack is empty. Cannot pop.\n");
                }
                displayStack(stack);
                break;
            case 3:
                if (!isEmpty(stack)) {
                    switch (stack->top->type) {
                        case 1:
                            printf("Top element: %d \n", peek(stack).intData);
                            break;
                        case 2:
                            printf("Top element: %c \n", peek(stack).charData);
                            break;
                        case 3:
                            printf("Top element: %f \n", peek(stack).floatData);
                            break;

                    }
                } else {
                    printf("Stack is empty. Cannot peek.\n");
                }
                break;
            case 4:
                if (isEmpty(stack)) {
                    printf("Stack is empty.\n");
                } else {
                    printf("Stack is not empty.\n");
                }
                break;
            case 5:
                if (isFull(stack)) {
                    printf("Stack is full.\n");
                } else {
                    printf("Stack is not full.\n");
                }
                break;
            case 6:
                freeStack(stack);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
