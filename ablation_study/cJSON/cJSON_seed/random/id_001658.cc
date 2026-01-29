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
//<ID> 1658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool was_false = 0;
    int size_before = 0;
    int size_after = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateFalse();
    item1 = cJSON_CreateString("stay");
    item2 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    added_arr = cJSON_AddItemToObject(root, "array", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    was_false = cJSON_IsFalse(item0);
    replacement = cJSON_CreateString("replaced");
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, replacement);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation = (int)added0 + (int)added1 + (int)added2 + (int)added_arr + (int)replaced_ok + (int)(size_before == 3) + (int)(size_after == 3) + (int)was_false;

    // step 6: Cleanup
    json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_free((void *)json_unformatted);
    cJSON_Delete(root);
    (void)validation;
    (void)item0;
    (void)item1;
    (void)item2;
    (void)replacement;
    // API sequence test completed successfully
    return 66;
}