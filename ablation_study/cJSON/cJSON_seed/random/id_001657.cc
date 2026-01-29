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
//<ID> 1657
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
    cJSON *item_true = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *item_number = (cJSON *)0;
    cJSON *repl_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    cJSON_bool added_number = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_true = cJSON_CreateTrue();
    item_false = cJSON_CreateFalse();
    item_number = cJSON_CreateNumber(42.0);
    repl_item = cJSON_CreateString("replaced");

    // step 3: Configure
    added_true = cJSON_AddItemToArray(arr, item_true);
    added_false = cJSON_AddItemToArray(arr, item_false);
    added_number = cJSON_AddItemToArray(arr, item_number);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInArray(arr, 1, repl_item);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation = (int)size + (int)added_true + (int)added_false + (int)added_number + (int)replaced + (int)(!is_false) + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)validation;
    (void)item_false;
    (void)item_true;
    (void)item_number;
    (void)retrieved;

    // API sequence test completed successfully
    return 66;
}