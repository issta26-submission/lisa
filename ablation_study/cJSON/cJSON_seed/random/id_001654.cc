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
//<ID> 1654
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
    cJSON *a1 = (cJSON *)0;
    cJSON *a2 = (cJSON *)0;
    cJSON *a3 = (cJSON *)0;
    cJSON *new_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool added_arrobj = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool is_false = 0;
    int array_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    a1 = cJSON_CreateString("one");
    a2 = cJSON_CreateFalse();
    a3 = cJSON_CreateString("three");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, a1);
    added2 = cJSON_AddItemToArray(arr, a2);
    added3 = cJSON_AddItemToArray(arr, a3);
    added_arrobj = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    new_item = cJSON_CreateString("replaced");
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, new_item);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)added1 + (int)added2 + (int)added3 + (int)added_arrobj + (int)replaced_ok + (int)(array_size == 3) + (int)is_false;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)validation;
    (void)retrieved;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)new_item;
    // API sequence test completed successfully
    return 66;
}