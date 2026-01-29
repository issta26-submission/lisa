// Header
#include <cstddef>
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include <cassert>

int test_api_sequence() {
    // step 1: Declarations & Initialize
    cJSON *str_item = cJSON_CreateString("Hello World");
    assert(str_item != NULL);
    assert(str_item != nullptr);
    assert(str_item->type == cJSON_String);
    assert((str_item->type & 0xFF) == cJSON_String);
    assert(str_item->valuestring != NULL);
    assert(str_item->valuestring != nullptr);
    assert(strlen(str_item->valuestring) > 0);
    assert(strlen(str_item->valuestring) == strlen("Hello World"));
    assert(strcmp(str_item->valuestring, "Hello World") == 0);
    assert(str_item->child == NULL);
    assert(str_item->next == NULL);
    assert(str_item->prev == NULL);
    assert(str_item->valuestring[0] == 'H');
    assert(str_item->valuestring[5] == ' ');
    assert(str_item->valuestring[strlen(str_item->valuestring) - 1] == 'd');
    // step 2: Configure & Trigger
    char *old_ptr = str_item->valuestring;
    assert(old_ptr != NULL);
    assert(old_ptr == str_item->valuestring);
    assert(strcmp(old_ptr, "Hello World") == 0);
    char *result = cJSON_SetValuestring(str_item, NULL);
    // step 3: Operate & Validate
    assert(result == NULL);
    assert(str_item != NULL);
    assert(str_item != nullptr);
    assert(str_item->type == cJSON_String);
    assert((str_item->type & 0xFF) == cJSON_String);
    assert(str_item->valuestring != NULL);
    assert(str_item->valuestring == old_ptr);

    assert(strlen(str_item->valuestring) == strlen("Hello World"));
    assert(strcmp(str_item->valuestring, "Hello World") == 0);
    assert(str_item->valuestring[0] == 'H');
    assert(str_item->valuestring[1] == 'e');
    assert(str_item->valuestring[10] == 'd');
    assert(str_item->child == NULL);
    assert(str_item->next == NULL);
    assert(str_item->prev == NULL);
    assert(old_ptr == str_item->valuestring);
    assert(memcmp(str_item->valuestring, "Hello World",
                  strlen("Hello World")) == 0);


    // step 4: Cleanup
    assert(str_item != NULL);
    assert(str_item->valuestring != NULL);
    assert(strcmp(str_item->valuestring, "Hello World") == 0);
    assert((str_item->type & 0xFF) == cJSON_String);
    cJSON_Delete(str_item);
    str_item = NULL;
    assert(str_item == NULL);
    assert(str_item == nullptr);

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
