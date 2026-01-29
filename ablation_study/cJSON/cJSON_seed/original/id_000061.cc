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
//<ID> 61
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "node");
    cJSON_AddNumberToObject(child, "value", 123.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, child);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, ref);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "note", "extra");
    cJSON_AddItemToArray(arr, extra);

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool equal = cJSON_Compare(item0, item1, 1);
    cJSON_bool item0_is_obj = cJSON_IsObject(item0);
    cJSON_bool item1_is_obj = cJSON_IsObject(item1);
    cJSON *val_item = cJSON_GetObjectItem(item0, "value");
    double val = cJSON_GetNumberValue(val_item);
    int summary = (int)equal + (int)item0_is_obj + (int)item1_is_obj + (int)val;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}