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
//<ID> 585
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "value", 42.5);
    cJSON_AddStringToObject(child, "label", "example");
    cJSON *cs_item = cJSON_CreateString("cs_value");
    cJSON_AddItemToObjectCS(root, "cs_key", cs_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(child, "items", arr);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_child = cJSON_GetObjectItem(dup, "child");
    cJSON *dup_items = cJSON_GetObjectItem(dup_child, "items");
    int size = cJSON_GetArraySize(dup_items);
    cJSON *dup_value_item = cJSON_GetObjectItem(dup_child, "value");
    double value = cJSON_GetNumberValue(dup_value_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;size=%d;val=%.1f",
            version ? version : "null",
            size,
            value);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}