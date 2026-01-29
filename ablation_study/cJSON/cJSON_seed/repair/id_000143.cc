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
//<ID> 143
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(items, n0);
    cJSON *s1 = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(items, s1);
    cJSON *b2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, b2);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "root_meta");

    // step 2: Configure
    cJSON_AddStringToObject(root, "library_version", cJSON_Version());
    cJSON_AddStringToObject(root, "source_tag", "generated");
    cJSON_AddNumberToObject(root, "initial_scale", 1.0);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(out);
    cJSON_free(out);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int item_count = cJSON_GetArraySize(parsed_items);
    cJSON *p0 = cJSON_GetArrayItem(parsed_items, 0);
    cJSON *p1 = cJSON_GetArrayItem(parsed_items, 1);
    cJSON *p2 = cJSON_GetArrayItem(parsed_items, 2);
    double v0 = cJSON_GetNumberValue(p0);
    const char *v1 = cJSON_GetStringValue(p1);
    cJSON_bool v2 = cJSON_IsTrue(p2);
    cJSON_AddNumberToObject(root, "first_value", v0);
    cJSON_AddStringToObject(root, "second_value", v1 ? v1 : "");
    cJSON_AddBoolToObject(root, "third_value", v2);
    cJSON_AddNumberToObject(root, "item_count", item_count);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}