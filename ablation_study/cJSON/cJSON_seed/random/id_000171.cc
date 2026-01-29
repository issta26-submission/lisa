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
//<ID> 171
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
    cJSON *s_first = (cJSON *)0;
    cJSON *s_third = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *inserted_at_one = (cJSON *)0;
    cJSON *num_item_is_bool = (cJSON *)0;
    cJSON *json_text = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool insert_ok = (cJSON_bool)0;
    cJSON_bool item1_is_bool = (cJSON_bool)0;
    cJSON_bool item1_is_true = (cJSON_bool)0;
    int arr_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s_first = cJSON_CreateString("one");
    s_third = cJSON_CreateString("three");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s_first);
    add_ok2 = cJSON_AddItemToArray(arr, s_third);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddBoolToObject(root, "flag_always_true", 1);

    // step 4: Operate
    insert_ok = cJSON_InsertItemInArray(arr, 1, true_item);
    arr_size = cJSON_GetArraySize(arr);
    inserted_at_one = cJSON_GetArrayItem(arr, 1);

    // step 5: Validate
    item1_is_bool = cJSON_IsBool(inserted_at_one);
    item1_is_true = cJSON_IsTrue(inserted_at_one);
    num_item_is_bool = cJSON_CreateNumber((double)item1_is_bool);
    cJSON_AddItemToObject(root, "item1_is_bool_num", num_item_is_bool);
    cJSON_AddBoolToObject(root, "item1_is_true_flag", item1_is_true);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}