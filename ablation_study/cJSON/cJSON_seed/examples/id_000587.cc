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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, item_a);
    cJSON_AddItemToArray(array, item_b);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "names", array);
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObjectCS(root, "names_copy", dup_array);
    cJSON_AddNumberToObject(root, "original_count", (double)cJSON_GetArraySize(array));
    cJSON_AddNumberToObject(root, "copy_count", (double)cJSON_GetArraySize(dup_array));
    cJSON_AddStringToObject(root, "description", "demo");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    int orig_size = cJSON_GetArraySize(array);
    int copy_size = cJSON_GetArraySize(dup_array);
    char status[256];
    memset(status, 0, sizeof(status));
    const char *desc = cJSON_GetStringValue(cJSON_GetObjectItem(root, "description"));
    sprintf(status, "ver=%s;orig=%d;copy=%d;desc=%s",
            version ? version : "null",
            orig_size,
            copy_size,
            desc ? desc : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}