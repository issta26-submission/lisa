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
//<ID> 2403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting_item = cJSON_AddStringToObject(root, "greeting", "hello");
    const char *json_obj = "{\"num\":123}";
    cJSON *parsed_obj = cJSON_Parse(json_obj);
    const char *json_str = "\"world\"";
    const char *parse_end = NULL;
    cJSON *parsed_str = cJSON_ParseWithOpts(json_str, &parse_end, 1);

    // step 2: Configure
    cJSON_bool greeting_is_string = cJSON_IsString(greeting_item);
    cJSON_bool parsed_str_is_invalid = cJSON_IsInvalid(parsed_str);
    cJSON_bool parsed_str_is_string = cJSON_IsString(parsed_str);

    // step 3: Operate
    cJSON_AddItemToObject(root, "object", parsed_obj);
    cJSON_AddItemToObject(root, "parsed_string", parsed_str);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    buffer[1] = (char)(int)greeting_is_string;
    buffer[2] = (char)(int)parsed_str_is_invalid;
    buffer[3] = (char)(int)parsed_str_is_string;
    buffer[4] = printed[0];

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}