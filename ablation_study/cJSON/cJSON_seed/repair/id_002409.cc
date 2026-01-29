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
//<ID> 2409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item1 = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *item2 = cJSON_AddStringToObject(root, "target", "world");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    const char *parse_end = NULL;
    cJSON *parsed_via_parse = cJSON_Parse(printed);
    cJSON *parsed_via_opts = cJSON_ParseWithOpts(printed, &parse_end, 1);
    cJSON *greeting_item = cJSON_GetObjectItem(parsed_via_opts, "greeting");
    cJSON_bool greeting_invalid = cJSON_IsInvalid(greeting_item);
    cJSON_bool greeting_is_string = cJSON_IsString(greeting_item);
    char *greeting_val = cJSON_GetStringValue(greeting_item);
    buffer[1] = (char)(int)greeting_invalid;
    buffer[2] = (char)(int)greeting_is_string;
    buffer[3] = greeting_val && greeting_val[0] ? greeting_val[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(parsed_via_opts);
    buffer[4] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed_via_parse);
    cJSON_Delete(parsed_via_opts);
    // API sequence test completed successfully
    return 66;
}