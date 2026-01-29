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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddStringToObject(config, "name", "example");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateString("two"));
    cJSON_AddItemToArray(items, cJSON_CreateString("three"));
    cJSON *arr_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, arr_true);
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    const char *s0 = cJSON_GetStringValue(it0);
    const char *s1 = cJSON_GetStringValue(it1);
    cJSON_AddStringToObject(config, "first_item", s0);
    cJSON_AddStringToObject(config, "second_item", s1);
    cJSON_AddNumberToObject(root, "items_count", (double)count);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}