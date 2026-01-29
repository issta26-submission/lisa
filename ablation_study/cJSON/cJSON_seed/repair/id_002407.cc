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
//<ID> 2407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_with_trailing = "{\"alpha\":\"A\"}TRAIL";
    const char *json_simple = "{\"beta\":\"B\"}";
    const char *parse_end = NULL;
    cJSON *parsed_with_trailing = cJSON_ParseWithOpts(json_with_trailing, &parse_end, 0);
    cJSON *parsed_simple = cJSON_Parse(json_simple);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "p1", parsed_with_trailing);
    cJSON_AddItemToObject(root, "p2", parsed_simple);

    // step 2: Configure
    char *printed_before = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *added = cJSON_AddStringToObject(root, "added", "newvalue");
    cJSON *found_p1 = cJSON_GetObjectItem(root, "p1");
    cJSON *alpha_item = cJSON_GetObjectItem(found_p1, "alpha");
    cJSON_bool alpha_is_string = cJSON_IsString(alpha_item);
    cJSON_bool alpha_is_invalid = cJSON_IsInvalid(alpha_item);
    buffer[0] = printed_before[0];
    buffer[1] = (char)(int)alpha_is_string;
    buffer[2] = (char)(int)alpha_is_invalid;
    buffer[10] = (char)(int)(parse_end ? *parse_end : 0);

    // step 4: Validate and Cleanup
    char *printed_after = cJSON_PrintUnformatted(root);
    buffer[3] = printed_after[0];
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}