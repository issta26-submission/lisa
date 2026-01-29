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
//<ID> 2400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON *age_item = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root, "age", age_item);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *parsed = cJSON_Parse(printed);
    const char *parse_end = NULL;
    cJSON *parsed_opts = cJSON_ParseWithOpts(printed, &parse_end, 1);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON_bool is_name_string = cJSON_IsString(parsed_name);
    cJSON_bool is_root_invalid = cJSON_IsInvalid(root);
    cJSON_bool is_parsed_opts_invalid = cJSON_IsInvalid(parsed_opts);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    buffer[1] = (char)(int)is_name_string;
    buffer[2] = (char)(int)is_root_invalid;
    buffer[3] = (char)(int)is_parsed_opts_invalid;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(parsed_opts);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}