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
//<ID> 223
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
    cJSON *bool_item = cJSON_CreateFalse();
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(arr, bool_item);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);

    // step 3: Operate and Validate
    cJSON_bool was_false = cJSON_IsFalse(detached);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "moved", was_false);
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    int summary = (int)ref_added + (int)was_false + arr_size + (printed ? (int)printed[0] : 0);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}