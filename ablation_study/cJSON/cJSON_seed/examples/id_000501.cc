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
//<ID> 501
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, n1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", version ? version : "unknown");
    cJSON_AddItemToObject(root, "meta", meta);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    double num_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    sprintf(status_buf, "first=%s;num=%.0f", first_str ? first_str : "null", num_val);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);
    cJSON_AddItemToArray(items, cJSON_CreateString("added-item"));

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}