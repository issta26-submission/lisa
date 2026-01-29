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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_test");
    cJSON *active_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active_flag);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, it0);
    cJSON *it1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(items, it2);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddStringToObject(config, "mode", "fast");
    cJSON_AddStringToObject(config, "origin", cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *first_text = cJSON_GetStringValue(first);
    double second_val = cJSON_GetNumberValue(second);
    double summary = second_val + (double)count;
    cJSON_AddNumberToObject(root, "summary", summary);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)first_text;
    return 66;
    // API sequence test completed successfully
}