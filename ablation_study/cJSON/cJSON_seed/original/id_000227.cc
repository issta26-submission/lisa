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
//<ID> 227
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", num);
    cJSON *status = cJSON_AddBoolToObject(root, "status", 0);
    cJSON *ref_item = cJSON_CreateString("shared_value");
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON *extra = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, extra);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "detached_ref", detached);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    double num_val = cJSON_GetNumberValue(num);
    cJSON_bool is_false = cJSON_IsFalse(status);
    const char *det_str = cJSON_GetStringValue(detached);
    int summary = arr_size + (int)num_val + (is_false ? 1 : 0) + (det_str ? (int)det_str[0] : 0);
    void *tmp = cJSON_malloc(32);
    memset(tmp, 0, 32);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}