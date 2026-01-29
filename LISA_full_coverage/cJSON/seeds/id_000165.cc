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
//<ID> 165
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, t1);
    cJSON_AddItemToArray(arr, f1);

    // step 2: Configure
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 1, t2);
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_bool detached_is_bool = cJSON_IsBool(detached);
    cJSON *first_in_arr = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_in_arr);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int summary = (int)detached_is_bool + (int)first_is_bool + root->type + meta->type + (buffer ? (int)buffer[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}