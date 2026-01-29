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
//<ID> 2402
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

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    const char *endptr = NULL;
    cJSON *parsed_with_opts = cJSON_ParseWithOpts(printed, &endptr, 1);
    cJSON *parsed_simple = cJSON_Parse(printed);
    cJSON *greet_from_parsed = cJSON_GetObjectItem(parsed_simple, "greeting");
    cJSON_bool orig_is_string = cJSON_IsString(greeting_item);
    cJSON_bool parsed_is_string = cJSON_IsString(greet_from_parsed);
    cJSON_bool parsed_is_invalid = cJSON_IsInvalid(greet_from_parsed);
    buffer[1] = (char)((int)orig_is_string + '0');
    buffer[2] = (char)((int)parsed_is_string + '0');
    buffer[3] = (char)((int)parsed_is_invalid + '0');

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed_with_opts);
    cJSON_Delete(parsed_simple);
    // API sequence test completed successfully
    return 66;
}