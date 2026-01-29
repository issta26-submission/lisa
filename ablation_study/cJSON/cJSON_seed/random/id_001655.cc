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
//<ID> 1655
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
    cJSON *b_true = (cJSON *)0;
    cJSON *b_false = (cJSON *)0;
    cJSON *replacement_false = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_out = (char *)0;
    cJSON_bool added_first = 0;
    cJSON_bool added_second = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int array_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    b_true = cJSON_CreateTrue();
    b_false = cJSON_CreateFalse();
    replacement_false = cJSON_CreateFalse();

    // step 3: Configure
    added_first = cJSON_AddItemToArray(arr, b_true);
    added_second = cJSON_AddItemToArray(arr, b_false);
    (void)cJSON_AddItemToObject(root, "flags", arr);

    // step 4: Operate
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement_false);
    array_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    json_out = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)added_first + (int)added_second + (int)replaced + (int)(array_size == 2) + (int)is_false + (int)(json_out != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_out);
    cJSON_Delete(root);
    (void)validation;
    (void)retrieved;
    (void)replacement_false;
    (void)b_true;
    (void)b_false;
    // API sequence test completed successfully
    return 66;
}