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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"enabled\":true,\"count\":42}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool enabled_val = cJSON_IsTrue(parsed_enabled);
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(parsed_count);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON *enabled_copy = cJSON_CreateBool(enabled_val);
    cJSON *count_copy = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "name", name_copy);
    cJSON_AddItemToObject(root, "enabled", enabled_copy);
    cJSON_AddItemToObject(root, "count", count_copy);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(parsed, root, 1);
    cJSON *comparison_marker = cJSON_CreateString(equal ? "equal" : "not_equal");
    cJSON_AddItemToObject(root, "comparison", comparison_marker);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}