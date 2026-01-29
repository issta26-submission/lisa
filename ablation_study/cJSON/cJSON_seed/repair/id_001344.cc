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
//<ID> 1344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *arr = cJSON_CreateArray();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON *str_ref = cJSON_CreateStringReference("detached_string");
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToObject(root, "misc", arr);

    // step 2: Configure
    cJSON *label_ref = cJSON_CreateStringReference("root_label");
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(arr, str_ref);
    int size_before = cJSON_GetArraySize(ints);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(ints, 0));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *det_str = cJSON_GetStringValue(detached);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = det_str ? det_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_before % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)first_val % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}