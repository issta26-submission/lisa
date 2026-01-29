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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"num\": 42.5, \"flag\": false, \"name\": \"example\"}";
    cJSON *root = (cJSON *)0;
    const char *parse_end = (const char *)0;
    const char **return_parse_end = &parse_end;
    cJSON_bool require_null_terminated = (cJSON_bool)1;
    cJSON_bool is_obj = (cJSON_bool)0;
    cJSON *num_item = (cJSON *)0;
    double num_val = 0.0;
    cJSON *num_copy = (cJSON *)0;
    char *num_text = (char *)0;
    cJSON *name_item = (cJSON *)0;
    const char *name_val = (const char *)0;
    char *json_out = (char *)0;

    // step 2: Initialize (parse JSON with explicit length/options)
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), return_parse_end, require_null_terminated);

    // step 3: Configure (inspect and annotate the parsed object)
    is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "is_object", is_obj);

    // step 4: Operate (extract number, convert to string via cJSON_PrintUnformatted, and copy name)
    num_item = cJSON_GetObjectItem(root, "num");
    num_val = cJSON_GetNumberValue(num_item);
    num_copy = cJSON_CreateNumber(num_val);
    num_text = cJSON_PrintUnformatted(num_copy);
    cJSON_AddStringToObject(root, "num_as_string", num_text);
    cJSON_free(num_text);
    cJSON_Delete(num_copy);
    name_item = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "copied_name", name_val);

    // step 5: Validate (serialize and release serialized buffer)
    json_out = cJSON_PrintUnformatted(root);
    cJSON_free(json_out);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}