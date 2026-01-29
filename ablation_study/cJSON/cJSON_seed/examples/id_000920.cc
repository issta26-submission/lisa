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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *label = cJSON_CreateString("old-label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *new_label = cJSON_CreateString("label-updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "label", new_label);

    // step 3: Operate & Validate
    cJSON *num_in_arr = cJSON_GetArrayItem(arr, 1);
    cJSON_SetNumberHelper(num_in_arr, 42.0);
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    char *label_text = cJSON_GetStringValue(got_label);
    cJSON *label_copy = cJSON_CreateString(label_text);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}