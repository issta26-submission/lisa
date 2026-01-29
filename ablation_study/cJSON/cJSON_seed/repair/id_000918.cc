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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *old_label = cJSON_CreateString("old_label_value");
    cJSON_AddItemToObject(meta, "old_label", old_label);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, flag);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Operate and Validate
    int initial_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)initial_count);
    cJSON *replacement = cJSON_CreateString("new_label_value");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(meta, "old_label", replacement);
    (void)replaced;
    cJSON *detached_name = cJSON_DetachItemFromObject(meta, "name");
    int post_count = cJSON_GetArraySize(items);
    (void)post_count;
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 3: Cleanup
    cJSON_Delete(detached_name);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}