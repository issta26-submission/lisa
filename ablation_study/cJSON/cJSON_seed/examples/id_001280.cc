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
//<ID> 1280
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
    char *dynamic_str = (char*)cJSON_malloc(16);
    memset(dynamic_str, 0, 16);
    memcpy(dynamic_str, "dynamic", 8);
    cJSON *str_dyn = cJSON_CreateString(dynamic_str); // create copies the string
    cJSON *str_fixed = cJSON_CreateString("fixed");

    // step 2: Configure
    cJSON_AddItemToArray(items, str_dyn);
    cJSON_AddItemToArray(items, str_fixed);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *items_ref = cJSON_Duplicate(items, 1); // deep-duplicate to avoid double-free
    cJSON_AddItemToObject(root, "items_ref", items_ref);

    // step 3: Operate & Validate
    char *out = cJSON_Print(root);
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    cJSON_AddItemToObject(root, "first_item_copy", cJSON_CreateString(first_val));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(dynamic_str);

    // API sequence test completed successfully
    return 66;
}