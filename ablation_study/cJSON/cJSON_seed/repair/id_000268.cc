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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 42, \"replace_me\": 7.5}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(123.456);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);
    cJSON *extra = cJSON_CreateNumber(88.8);
    cJSON_bool added = cJSON_AddItemToObject(root, "added", extra);

    // step 3: Operate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON *value_copy = cJSON_CreateNumber(value_num);
    cJSON_AddItemToObject(root, "value_copy", value_copy);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parse_end;
    (void)replaced;
    (void)added;
    return 66;
}