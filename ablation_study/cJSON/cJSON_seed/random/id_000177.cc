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
//<ID> 177
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
    cJSON *true_item = (cJSON *)0;
    cJSON *mid_item = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size = 0;
    cJSON_bool inserted_a = (cJSON_bool)0;
    cJSON_bool inserted_b = (cJSON_bool)0;
    cJSON_bool inserted_c = (cJSON_bool)0;
    cJSON_bool mid_is_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateNumber(42.0);
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    inserted_a = cJSON_InsertItemInArray(arr, 0, item0);
    inserted_b = cJSON_InsertItemInArray(arr, 1, item1);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    inserted_c = cJSON_InsertItemInArray(arr, 1, true_item);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    mid_item = cJSON_GetArrayItem(arr, 1);
    mid_is_true = cJSON_IsTrue(mid_item);
    cJSON_AddBoolToObject(root, "inserted_a", inserted_a);
    cJSON_AddBoolToObject(root, "inserted_b", inserted_b);
    cJSON_AddBoolToObject(root, "inserted_c", inserted_c);
    cJSON_AddBoolToObject(root, "middle_is_true", mid_is_true);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}