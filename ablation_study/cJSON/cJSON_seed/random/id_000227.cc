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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_literal[] = "{\"key\": 42, \"name\":\"example\"}";
    size_t buffer_length = sizeof(json_literal) - 1;
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *key_item = (cJSON *)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    double key_value = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize / Parse
    root = cJSON_ParseWithLengthOpts(json_literal, buffer_length, &parse_end, (cJSON_bool)1);

    // step 3: Configure
    is_obj = cJSON_IsObject((const cJSON *const)root);
    cJSON_AddBoolToObject(root, "is_object", is_obj);
    cJSON_AddStringToObject(root, "source", "inline_parse");

    // step 4: Operate
    key_item = cJSON_GetObjectItem(root, "key");
    key_value = cJSON_GetNumberValue((const cJSON *const)key_item);
    cJSON_AddNumberToObject(root, "key_copy", key_value);

    // step 5: Validate (serialize and free)
    printed = cJSON_PrintUnformatted((const cJSON * )root);
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}