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
//<ID> 915
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
    cJSON *status = cJSON_CreateString("initial_status");
    cJSON_AddItemToObject(root, "status", status);
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, num0);
    cJSON *flag0 = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, flag0);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON *replacement = cJSON_CreateString("replacement_status");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "status", replacement);
    (void)replaced_ok;

    // step 3: Operate and Validate
    cJSON *detached_status = cJSON_DetachItemFromObject(root, "status");
    cJSON *flag1 = cJSON_CreateBool(0);
    cJSON_AddItemToArray(items, flag1);
    int size_after = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(detached_status);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}