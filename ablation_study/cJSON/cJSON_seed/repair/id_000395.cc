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
//<ID> 395
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
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 0, str1);
    cJSON_InsertItemInArray(arr, 1, str2);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *label = cJSON_CreateString("label_value");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_SetValuestring(detached, "second_detached");
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, inserted);
    cJSON_SetValuestring(inserted, "inserted_modified");
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}