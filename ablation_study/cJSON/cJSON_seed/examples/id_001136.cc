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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[] = {1.5, 2.5, 3.5};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *meta = cJSON_CreateObject();
    cJSON *ident = cJSON_CreateString("item_42");
    cJSON *score = cJSON_CreateNumber(88.8);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", double_array);
    cJSON_AddItemToObjectCS(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "id", ident);
    cJSON_AddItemToObjectCS(meta, "score", score);

    // step 3: Operate & Validate
    cJSON *vals = cJSON_GetObjectItem(root, "values");
    cJSON *first = cJSON_GetArrayItem(vals, 0);
    cJSON *second = cJSON_GetArrayItem(vals, 1);
    double s = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "partial_sum", s);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    cJSON_AddBoolToObject(root, "confirmed_enabled", enabled_is_true);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}