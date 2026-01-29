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
//<ID> 1575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums_arr[4] = {1, 2, 3, 4};
    cJSON *array = cJSON_CreateIntArray(nums_arr, 4);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddStringToObject(root, "note", "sample");
    cJSON *temp = cJSON_CreateString("to_remove");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_enabled = cJSON_Duplicate(cJSON_GetObjectItem(root, "enabled"), 0);
    cJSON *first_elem = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    char *printed = cJSON_PrintUnformatted(dup_root);
    cJSON_DeleteItemFromObject(root, "temp");

    // step 3: Operate & Validate
    double version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    cJSON *computed = cJSON_CreateNumber(first_val + version_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *copy_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", copy_array);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(dup_enabled);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}