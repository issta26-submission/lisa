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
//<ID> 208
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items_copy");
    cJSON *first = cJSON_GetArrayItem(retrieved, 0);
    cJSON *second = cJSON_GetArrayItem(retrieved, 1);
    int validation_score = (int)cJSON_IsFalse(first) + (int)cJSON_IsNull(second);
    char *out = cJSON_PrintUnformatted(root);
    (void)validation_score;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}