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
//<ID> 1656
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool is_false = 0;
    int array_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("first");
    item2 = cJSON_CreateString("second");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 4: Operate
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, false_item);
    array_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)added1 + (int)added2 + (int)replaced_ok + (int)(array_size == 2) + (int)is_false + (int)(json_unformatted != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)validation;
    (void)retrieved;
    (void)item1;
    (void)item2;
    (void)false_item;
    // API sequence test completed successfully
    return 66;
}