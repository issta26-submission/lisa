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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    char *printed = (char *)0;
    const char json[] = "{\"value\": 42.5, \"name\": \"example\"}";
    size_t json_len = (size_t)(sizeof(json) - 1);
    const char *parse_end = (const char *)0;
    cJSON_bool require_null = (cJSON_bool)1;
    cJSON_bool is_obj = (cJSON_bool)0;
    double extracted_value = 0.0;
    const char *name_text = (const char *)0;

    // step 2: Initialize / Parse
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, require_null);

    // step 3: Configure parsed structure
    is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "is_object", is_obj);
    value_item = cJSON_GetObjectItem(root, "value");
    name_item = cJSON_GetObjectItem(root, "name");

    // step 4: Operate on parsed values
    extracted_value = cJSON_GetNumberValue(value_item);
    name_text = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "orig_name", name_text);
    cJSON_AddStringToObject(root, "cjson_version", cJSON_Version());

    // step 5: Validate by producing output
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}