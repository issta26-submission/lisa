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
//<ID> 200
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, f);
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(items, n);
    cJSON_AddStringToObject(meta, "status", "initialized");
    cJSON_AddNumberToObject(meta, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON *items_copy = cJSON_Duplicate(retrieved_items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON *copied_first = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items_copy"), 0);
    cJSON_bool is_false_flag = cJSON_IsFalse(copied_first);
    (void)is_false_flag;
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    (void)retrieved_meta;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}