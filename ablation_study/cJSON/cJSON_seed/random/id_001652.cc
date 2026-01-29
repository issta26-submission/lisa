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
//<ID> 1652
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
    cJSON *item_false = (cJSON *)0;
    cJSON *item_true = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_true = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int size_before = 0;
    int size_after = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_false = cJSON_CreateFalse();
    item_true = cJSON_CreateTrue();
    replacement = cJSON_CreateString("replacement");

    // step 3: Configure
    added_false = cJSON_AddItemToArray(arr, item_false);
    added_true = cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    is_false = cJSON_IsFalse(item_false);
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    size_after = cJSON_GetArraySize(arr);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)added_false
               + (int)added_true
               + (int)is_false
               + (int)replaced
               + (int)(size_before == 2)
               + (int)(size_after == 2)
               + (int)(json_unformatted != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)validation;
    (void)size_before;
    (void)size_after;
    (void)item_false;
    (void)item_true;
    (void)replacement;
    (void)added_false;
    (void)added_true;
    (void)replaced;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}