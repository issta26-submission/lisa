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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 3.14, \"nested\": {\"a\":1}}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig = cJSON_GetNumberValue(value_item);
    cJSON *newobj = cJSON_CreateObject();
    cJSON *double_item = cJSON_CreateNumber(orig * 2.0);
    cJSON_AddItemToObject(newobj, "double", double_item);
    cJSON_AddItemToObject(root, "added", newobj);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "value", replacement);
    cJSON *val_after = cJSON_GetObjectItem(root, "value");
    double got = cJSON_GetNumberValue(val_after);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parse_end; (void)replaced; (void)orig; (void)got;
    return 66;
}