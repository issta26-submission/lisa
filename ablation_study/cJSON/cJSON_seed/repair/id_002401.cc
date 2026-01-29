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
//<ID> 2401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_obj_text = "{\"name\":\"Alice\",\"age\":30}";
    const char *parse_end = NULL;
    cJSON *parsed_obj = cJSON_ParseWithOpts(json_obj_text, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(parsed_obj, "name");
    cJSON *age_item = cJSON_GetObjectItem(parsed_obj, "age");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "orig_name", cJSON_GetStringValue(name_item));
    cJSON_AddStringToObject(root, "stage", "initialized");

    // step 2: Configure
    cJSON *single_str = cJSON_Parse("\"parsed_string\"");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_bool age_is_invalid = cJSON_IsInvalid(age_item);
    char *printed_root = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed_root && printed_root[0] ? printed_root[0] : '\0';
    buffer[1] = (char)(int)name_is_string;
    buffer[2] = (char)(int)age_is_invalid;

    // step 3: Operate
    cJSON_AddStringToObject(root, "from_single", cJSON_GetStringValue(single_str));
    char *printed_after = cJSON_PrintUnformatted(root);
    buffer[3] = printed_after && printed_after[0] ? printed_after[0] : '\0';
    cJSON *parsed_simple = cJSON_Parse("\"inline\"");
    cJSON_bool simple_is_string = cJSON_IsString(parsed_simple);
    buffer[4] = (char)(int)simple_is_string;

    // step 4: Validate and Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_after);
    cJSON_free(buffer);
    cJSON_Delete(parsed_obj);
    cJSON_Delete(single_str);
    cJSON_Delete(parsed_simple);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}