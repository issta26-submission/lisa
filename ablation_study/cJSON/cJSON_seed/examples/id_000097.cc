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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pool = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "pool", pool);

    // step 2: Configure
    cJSON *b1 = cJSON_CreateBool(1);
    cJSON *b2 = cJSON_CreateBool(0);
    cJSON_AddItemReferenceToArray(pool, b1);
    cJSON_AddItemReferenceToArray(pool, b2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(meta, "active", active);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(pool);
    cJSON *fetched_pool = cJSON_GetObjectItem(root, "pool");
    int size2 = cJSON_GetArraySize(fetched_pool);
    cJSON *count = cJSON_CreateNumber((double)(size + size2));
    cJSON_AddItemToObject(root, "count", count);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}