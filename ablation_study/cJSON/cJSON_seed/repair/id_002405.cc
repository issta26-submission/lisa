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
//<ID> 2405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddStringToObject(root, "language", "C++");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *parse_end = NULL;
    cJSON *parsed1 = cJSON_Parse(printed);
    cJSON *parsed2 = cJSON_ParseWithOpts(printed, &parse_end, (cJSON_bool)1);

    // step 3: Operate
    cJSON *g1 = cJSON_GetObjectItem(parsed1, "greeting");
    cJSON_bool g1_is_string = cJSON_IsString(g1);
    cJSON_bool parsed2_is_invalid = cJSON_IsInvalid(parsed2);
    char const *g1_str = cJSON_GetStringValue(g1);
    cJSON_AddStringToObject(parsed2, "status", "parsed");
    buffer[0] = (char)(int)g1_is_string;
    buffer[1] = (char)(int)parsed2_is_invalid;
    buffer[2] = g1_str ? g1_str[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(parsed2);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    // API sequence test completed successfully
    return 66;
}