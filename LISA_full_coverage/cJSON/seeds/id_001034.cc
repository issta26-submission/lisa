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
//<ID> 1034
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"nums\":[1,2,3],\"value\":42}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *out = cJSON_CreateObject();
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *nums_item = cJSON_GetObjectItem(root, "nums");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *name_copy = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON *nums_dup = cJSON_Duplicate(nums_item, 1);
    cJSON *raw_injected = cJSON_CreateRaw("UNPARSED_RAW_CONTENT");
    cJSON_AddItemToObject(out, "name_copied", name_copy);
    cJSON_AddItemToObject(out, "nums_copied", nums_dup);
    cJSON_AddItemToObject(out, "injected_raw", raw_injected);
    cJSON *mutable_str = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(mutable_str, "modified_string");
    cJSON_AddItemToObject(out, "modified", mutable_str);
    cJSON_AddItemToObject(out, "original_value", cJSON_CreateNumber(cJSON_GetNumberValue(value_item)));

    // step 3: Operate
    cJSON_bool raw_check = cJSON_IsRaw(cJSON_GetObjectItem(out, "injected_raw"));
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}