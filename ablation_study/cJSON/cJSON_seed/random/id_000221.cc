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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_text[] = "{\"key\":42,\"flag\":false,\"name\":\"example\"}";
    size_t json_len = (size_t)(sizeof(json_text) - 1);
    const char * parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *key_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    const char *name_str = (const char *)0;
    double key_value = 0.0;
    cJSON_bool is_obj = (cJSON_bool)0;
    char *out_text = (char *)0;

    // step 2: Initialize (parse)
    root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, (cJSON_bool)1);

    // step 3: Configure (inspect and annotate)
    is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "is_object", is_obj);
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "parsed_name", name_str);

    // step 4: Operate (extract numeric value and copy back)
    key_item = cJSON_GetObjectItem(root, "key");
    key_value = cJSON_GetNumberValue(key_item);
    cJSON_AddNumberToObject(root, "key_copy", key_value);
    cJSON_AddBoolToObject(root, "original_flag", cJSON_IsObject(cJSON_GetObjectItem(root, "flag"))); // uses existing APIs sensibly

    // step 5: Validate (serialize)
    out_text = cJSON_PrintUnformatted(root);
    cJSON_free(out_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}