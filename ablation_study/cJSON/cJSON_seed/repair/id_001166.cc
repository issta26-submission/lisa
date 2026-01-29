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
//<ID> 1166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *count = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "orphan", null_item);
    cJSON_AddItemToObject(meta, "count", count);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_bool is_obj = cJSON_IsObject(detached_meta);
    cJSON *count_ref = cJSON_GetObjectItem(detached_meta, "count");
    double newval = cJSON_SetNumberHelper(count_ref, 42.5);
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "orphan"));
    double combined = newval + (double)is_obj + (double)is_null;
    cJSON *sum_item = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToObject(root, "meta_restored", detached_meta);
    char *updated = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_free(updated);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}