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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *orig_ref = "original_ref";
    const char *new_ref = "replaced_ref";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *str_ref = cJSON_CreateStringReference(orig_ref);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON *null_item = cJSON_AddNullToObject(root, "nullable");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference(new_ref);
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());

    // step 3: Operate
    int count = cJSON_GetArraySize(arr);
    char *flat = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_PrintPreallocated(root, buf, 512, 1);
    cJSON_free(flat);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    (void)count;
    (void)null_item;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}