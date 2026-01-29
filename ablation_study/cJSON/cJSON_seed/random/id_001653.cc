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
//<ID> 1653
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
    cJSON *replacement = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int size_before = 0;
    int size_after = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    false_item = cJSON_CreateFalse();
    replacement = cJSON_CreateString("gamma");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    added3 = cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "letters", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    is_false = cJSON_IsFalse(false_item);
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    size_after = cJSON_GetArraySize(arr);
    json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_free((void *)json_unformatted);

    // step 5: Validate
    validation = (int)added1 + (int)added2 + (int)added3 + (int)is_false + (int)replaced + (int)(size_before == 3) + (int)(size_after == 3);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation;
    (void)item1;
    (void)item2;
    (void)false_item;
    (void)replacement;
    (void)added1;
    (void)added2;
    (void)added3;
    (void)replaced;
    (void)is_false;
    (void)size_before;
    (void)size_after;
    // API sequence test completed successfully
    return 66;
}