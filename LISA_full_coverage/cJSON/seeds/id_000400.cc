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
//<ID> 400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *first = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, first);
    cJSON *second = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, second);
    cJSON *orig = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "replace_me", orig);

    // step 2: Configure
    cJSON *second_lookup = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_lookup);
    double validity_flag = 1.0 - (double)second_invalid;
    cJSON *valid_flag_item = cJSON_CreateNumber(validity_flag);
    cJSON_AddItemToObject(root, "second_valid", valid_flag_item);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "val", 3.14159);
    cJSON_AddStringToObject(replacement, "note", "replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}