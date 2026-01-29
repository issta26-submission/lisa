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
//<ID> 1102
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
    cJSON *true_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_first = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    char *printed = (char *)0;
    const char *name_val = (const char *)0;
    cJSON_bool is_first_true = (cJSON_bool)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create a root object, an array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    name_item = cJSON_CreateString("device_ABC");

    // step 3: Configure - attach the true item into the array and attach both array and name to root
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate - fetch the array and its first element, and fetch the name string
    fetched_arr = cJSON_GetObjectItem(root, "flags");
    fetched_first = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_name = cJSON_GetObjectItem(root, "name");
    is_first_true = cJSON_IsTrue(fetched_first);
    name_val = cJSON_GetStringValue(fetched_name);
    arr_size = cJSON_GetArraySize(fetched_arr);

    // step 5: Validate - serialize root briefly and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_first != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(is_first_true) + (int)(name_val != (const char *)0) + (int)(arr_size == 1);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}