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
//<ID> 655
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n2);
    const char *label = "static_label";
    cJSON *sref = cJSON_CreateStringReference(label);
    cJSON_AddItemToObject(root, "label", sref);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_AddBoolToObject(meta, "first_is_number", first_is_number);
    cJSON *detached = cJSON_DetachItemViaPointer(root, sref);
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
    cJSON_AddBoolToObject(meta, "detached_is_invalid", detached_invalid);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}