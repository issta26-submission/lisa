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
//<ID> 1104
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
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    cJSON *fetched_true = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool is_true = (cJSON_bool)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("device_name");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - compose array and attach to root
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - fetch the array and its elements
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_str = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_true = cJSON_GetArrayItem(fetched_arr, 1);
    retrieved_str = cJSON_GetStringValue(fetched_str);
    is_true = cJSON_IsTrue(fetched_true);

    // step 5: Validate - serialize, measure array size and compute a small score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    arr_size = cJSON_GetArraySize(fetched_arr);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(fetched_true != (cJSON *)0) + (int)(retrieved_str != (const char *)0) + (int)(is_true) + (int)(arr_size == 2);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}