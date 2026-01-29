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
//<ID> 1178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":true,\"count\":7,\"nested\":{\"ok\":false}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *raw_item = cJSON_CreateRaw("{\"injected\":42}");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "explicit_true", true_item);
    cJSON_AddItemToObjectCS(root, "raw_added", raw_item);
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    cJSON_bool alpha_bool = cJSON_IsTrue(alpha_item);
    cJSON_AddBoolToObject(root, "alpha_copy", alpha_bool);

    // step 3: Operate & Validate
    cJSON *count_item = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *count_copy = cJSON_CreateNumber(count_val + 1.0);
    cJSON_AddItemToObjectCS(root, "count_plus_one", count_copy);
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");
    cJSON_bool nested_is_object = cJSON_IsObject(nested);
    cJSON_AddBoolToObject(root, "nested_is_object", nested_is_object);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}