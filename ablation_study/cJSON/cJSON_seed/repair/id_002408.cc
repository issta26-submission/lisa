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
//<ID> 2408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"name\":\"initial\",\"count\":10}";
    const char *json2 = "{\"embedded\":\"hello\"}EXTRA";
    cJSON *root1 = cJSON_Parse(json1);
    const char *parse_end = NULL;
    cJSON *root2 = cJSON_ParseWithOpts(json2, &parse_end, (cJSON_bool)0);
    cJSON *added_item = cJSON_AddStringToObject(root1, "added", "new_value");

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root1);
    char *printed2 = cJSON_PrintUnformatted(root2);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed1 ? printed1[0] : '\0';
    buffer[1] = printed2 ? printed2[0] : '\0';

    // step 3: Operate
    cJSON *name_item = cJSON_GetObjectItem(root1, "name");
    cJSON *embedded_item = cJSON_GetObjectItem(root2, "embedded");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_bool embedded_is_string = cJSON_IsString(embedded_item);
    cJSON_bool root2_is_invalid = cJSON_IsInvalid(root2);
    buffer[2] = (char)('0' + (int)name_is_string);
    buffer[3] = (char)('0' + (int)embedded_is_string);
    buffer[4] = (char)('0' + (int)root2_is_invalid);

    // step 4: Validate and Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    // API sequence test completed successfully
    return 66;
}