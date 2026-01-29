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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *item1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, item1);
    cJSON *item2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(list, item2);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", flag);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *old = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(meta, "key", old);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(list);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *detached = cJSON_DetachItemFromObject(root, "active");
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "key", replacement);

    // step 4: Cleanup
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}