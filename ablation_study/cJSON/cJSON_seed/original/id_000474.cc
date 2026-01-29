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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "collection");

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddTrueToObject(child, "enabled");
    cJSON *num = cJSON_CreateNumber(1.0);
    double original_num = cJSON_GetNumberValue(num);
    double updated_num = cJSON_SetNumberHelper(num, 42.42);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "item", child);
    cJSON_AddItemReferenceToArray(arr, child);
    cJSON_bool child_is_object = cJSON_IsObject(child);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *val_item = cJSON_GetObjectItem(first, "value");
    double retrieved_value = cJSON_GetNumberValue(val_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    double aggregate = original_num + updated_num + retrieved_value + (double)child_is_object;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    (void)aggregate;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}