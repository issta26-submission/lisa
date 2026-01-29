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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 2.71828);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, cJSON_CreateString("third"));
    cJSON *null_entry = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_entry);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("SECOND_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON *fetched_null = cJSON_GetObjectItem(root, "nullable");
    cJSON_bool is_null = cJSON_IsNull(fetched_null);
    (void)replaced;
    (void)equal;
    (void)is_null;

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}