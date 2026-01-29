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
//<ID> 1650
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
    cJSON *new_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_str = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("zero");
    item1 = cJSON_CreateFalse();
    item2 = cJSON_CreateNumber(2.0);
    new_item = cJSON_CreateFalse();

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInArray(arr, 1, new_item);
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)size + (int)replaced + (int)is_false + (int)added0 + (int)added1 + (int)added2 + (int)(json_str != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);
    (void)validation;
    (void)retrieved;
    // API sequence test completed successfully
    return 66;
}