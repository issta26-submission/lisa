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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *greeting_ref = NULL;
    cJSON *list = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *retrieved_greeting = NULL;
    cJSON *retrieved_list = NULL;
    cJSON *after_deletion = NULL;
    const char *greet_val = NULL;
    int arr_size = 0;
    char *buffer = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    greeting_ref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToObject(child, "greeting", greeting_ref);
    list = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "list", list);
    elem0 = cJSON_CreateStringReference("one");
    cJSON_AddItemToArray(list, elem0);
    elem1 = cJSON_CreateStringReference("two");
    cJSON_AddItemToArray(list, elem1);

    // step 3: Operate / Validate
    retrieved_greeting = cJSON_GetObjectItemCaseSensitive(child, "greeting");
    greet_val = cJSON_GetStringValue(retrieved_greeting);
    retrieved_list = cJSON_GetObjectItemCaseSensitive(child, "list");
    arr_size = cJSON_GetArraySize(retrieved_list);
    buffer = (char *)cJSON_malloc(8);
    memset(buffer, 0, 8);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = greet_val[0];
    cJSON_DeleteItemFromObjectCaseSensitive(child, "greeting");
    after_deletion = cJSON_GetObjectItemCaseSensitive(child, "greeting");
    (void)after_deletion;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)retrieved_greeting;
    (void)retrieved_list;
    (void)greet_val;
    (void)arr_size;
    return 66;
}