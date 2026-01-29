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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON_AddItemToArray(items, cJSON_CreateNull());
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_list = cJSON_AddArrayToObject(meta, "meta_list");
    cJSON_AddItemToArray(meta_list, cJSON_CreateNull());
    cJSON_AddItemToArray(meta_list, cJSON_CreateFalse());

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(retrieved);
    cJSON_AddNumberToObject(meta, "item_count", (double)count);
    cJSON *copied = cJSON_GetObjectItem(root, "items_copy");
    int copy_count = cJSON_GetArraySize(copied);
    char *out = cJSON_PrintUnformatted(root);
    (void)retrieved;
    (void)copy_count;
    (void)count;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}