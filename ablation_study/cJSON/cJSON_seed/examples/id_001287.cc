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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_AddItemToObject(root, "items_ref", items_ref);
    cJSON_AddStringToObject(meta, "label", "dataset");

    // step 3: Operate & Validate
    char *out = cJSON_Print(root);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    size_t copy_len = strlen(first_val) + 1;
    void *buffer = cJSON_malloc(copy_len);
    memset(buffer, 0, copy_len);
    memcpy(buffer, first_val, copy_len);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}