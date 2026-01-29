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
//<ID> 913
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
    cJSON *item_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(list, item_true);
    cJSON *item_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(list, item_false);
    cJSON *name = cJSON_CreateString("node_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *new_flag = cJSON_CreateBool(1);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_flag);
    (void)replaced;
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(list);
    cJSON_AddNumberToObject(root, "count", (double)count);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool preprinted = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    (void)preprinted;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(snapshot);
    cJSON_Delete(detached_name);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}