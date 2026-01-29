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
//<ID> 1490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child_arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(child_arr, str_item);
    cJSON_AddItemToObject(root, "values", child_arr);
    cJSON_AddItemReferenceToObject(root, "values_ref", child_arr);

    // step 2: Configure
    char *printed_root = cJSON_PrintUnformatted(root);
    cJSON *values_obj = cJSON_GetObjectItem(root, "values");
    cJSON *first_elem = cJSON_GetArrayItem(values_obj, 0);
    const char *first_str = cJSON_GetStringValue(first_elem);
    (void)first_str;

    // step 3: Operate & Validate
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON *dup_values = cJSON_GetObjectItem(duplicate, "values");
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    const char *dup_first_str = cJSON_GetStringValue(dup_first);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    (void)dup_first_str;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}