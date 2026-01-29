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
//<ID> 176
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
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON_bool inserted_ok = (cJSON_bool)0;
    cJSON *first_item = (cJSON *)0;
    cJSON_bool first_is_true = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("one");
    item_b = cJSON_CreateString("two");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToObject(root, "items", arr);
    inserted_ok = cJSON_InsertItemInArray(arr, 0, true_item);

    // step 4: Operate
    first_item = cJSON_GetArrayItem(arr, 0);
    first_is_true = cJSON_IsTrue(first_item);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "inserted", inserted_ok);
    cJSON_AddBoolToObject(root, "first_is_true", first_is_true);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}