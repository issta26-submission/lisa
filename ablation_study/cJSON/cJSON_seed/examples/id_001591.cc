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
//<ID> 1591
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, s0);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToObject(root, "letters", array);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    int size_before = cJSON_GetArraySize(array);
    cJSON *size_num = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToObject(root, "letters_count", size_num);
    cJSON *is_false_indicator = cJSON_CreateNumber((double)cJSON_IsFalse(flag));
    cJSON_AddItemToObject(root, "is_enabled_false", is_false_indicator);

    // step 3: Operate
    cJSON_DeleteItemFromArray(array, 1);
    int size_after = cJSON_GetArraySize(array);
    cJSON *new_size_num = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToObject(root, "letters_count_after_delete", new_size_num);

    // step 4: Cleanup & Validate
    cJSON *note = cJSON_CreateString("cleanup");
    cJSON_AddItemToObject(root, "note", note);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}