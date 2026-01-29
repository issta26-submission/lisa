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
//<ID> 201
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

    // step 2: Configure
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, f);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(items, nul);
    cJSON *fdup = cJSON_Duplicate(f, 0);
    cJSON_AddItemToArray(items, fdup);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(retrieved);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *dup_items = cJSON_Duplicate(retrieved, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}