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
//<ID> 640
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
    cJSON *first_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, first_num);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToArray(items, nullable);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(items, nullable, replacement);

    // step 2: Configure / Serialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int item_count = cJSON_GetArraySize(parsed_items);
    cJSON *p0 = cJSON_GetArrayItem(parsed_items, 0);
    double v0 = cJSON_GetNumberValue(p0);
    cJSON *p1 = cJSON_GetArrayItem(parsed_items, 1);
    const char *s1 = cJSON_GetStringValue(p1);
    (void)version;
    (void)printed;
    (void)item_count;
    (void)v0;
    (void)s1;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}