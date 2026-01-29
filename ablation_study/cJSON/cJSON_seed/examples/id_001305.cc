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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "note", "child object attached");

    // step 3: Operate & Validate
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_bool is_invalid = cJSON_IsInvalid(replacement);
    cJSON *invalid_num = cJSON_CreateNumber((double)is_invalid);
    cJSON_AddItemToObject(root, "replacement_was_invalid", invalid_num);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, printed, printed_len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}