// Header
#include <cJSON.h>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int test_api_sequence() {
    // step 1: Declarations & Initialize
    cJSON *root = cJSON_CreateArray();
    assert(root != NULL);
    assert(root != nullptr);
    assert((root->type & 0xFF) == cJSON_Array);
    assert(root->child == NULL);
    assert(root->next == NULL);
    assert(root->prev == NULL);
    // step 2: Configure
    cJSON *item = cJSON_CreateString("Hello,world!");
    assert(item != NULL);
    assert((item->type & 0xFF) == cJSON_String);
    assert(item->valuestring != NULL);
    assert(strcmp(item->valuestring, "Hello,world!") == 0);
    assert(item->child == NULL);
    assert(item->next == NULL);
    assert(item->prev == NULL);
    cJSON_AddItemToArray(root, item);
    assert(root->child != NULL);
    assert(root->child == item);
    assert(item->next == NULL); 
    assert(item->child == NULL);
    assert(root->child->type == cJSON_String);
    assert(root->child->valuestring != NULL);
    assert(strcmp(root->child->valuestring, "Hello,world!") == 0);
    // step 3: Operate & Validate
    item->prev = NULL;
    assert(item->prev == NULL);
    cJSON *detached_item = cJSON_DetachItemViaPointer(root, item);
    assert(detached_item != NULL);
    assert(detached_item == item);
    assert(root->child == NULL);
    assert(detached_item->type == cJSON_String);
    assert(detached_item->valuestring != NULL);
    assert(strcmp(detached_item->valuestring, "Hello,world!") == 0);
    assert(detached_item->prev == NULL);
    assert(detached_item->next == NULL);
    assert(detached_item->child == NULL);
    detached_item->valuestring[0] = 'h';
    assert(root->child == NULL);
    assert(strcmp(detached_item->valuestring, "hello,world!") == 0);
    // step 4: Cleanup
    assert(detached_item != NULL);
    assert(root != NULL);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    detached_item = NULL;
    root = NULL;
    assert(detached_item == NULL);
    assert(root == NULL);
    cJSON *null_item = NULL;
    assert(null_item == NULL);
    assert(cJSON_DetachItemViaPointer(root, null_item) == NULL);
    // API sequence test completed successfully
    return 66;
}

int main() {
    for (int i = 0; i < 10; i++) {
        int result = test_api_sequence();
        if (result != 66) {
            std::cerr << "API sequence test failed with error code: "
                      << result << std::endl;
            exit(100);
        }
    }
    printf("Test completed successfully.\n");
    return 0;
}
