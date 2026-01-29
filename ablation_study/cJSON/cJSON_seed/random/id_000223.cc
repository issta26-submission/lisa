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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    char *serialized = (char *)0;
    const char *json_text = (const char *)0;
    const char *parse_end = (const char *)0;
    const char *extracted_name = (const char *)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    double extracted_num = 0.0;

    // step 2: Initialize
    json_text = "{\"num\": 42, \"name\": \"test\"}";
    parsed = cJSON_ParseWithLengthOpts(json_text, (size_t)1000, &parse_end, (cJSON_bool)1);

    // step 3: Configure
    is_obj = cJSON_IsObject(parsed);
    root = cJSON_CreateObject();
    cJSON_AddBoolToObject(root, "parsed_is_object", is_obj);

    // step 4: Operate
    num_item = cJSON_GetObjectItem(parsed, "num");
    extracted_num = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "extracted_num", extracted_num);
    name_item = cJSON_GetObjectItem(parsed, "name");
    extracted_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", extracted_name);

    // step 5: Validate
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}