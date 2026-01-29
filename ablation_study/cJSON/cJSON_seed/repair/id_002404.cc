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
//<ID> 2404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"Alice\",\"age\":30}";
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting_item = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *parsed1 = cJSON_Parse(json_text);
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(parsed1);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *name_item = cJSON_GetObjectItem(parsed1, "name");
    cJSON_bool invalid_parsed1 = cJSON_IsInvalid(parsed1);
    cJSON_bool invalid_parsed2 = cJSON_IsInvalid(parsed2);
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    char *name_value = cJSON_GetStringValue(name_item);

    // step 3: Operate
    buffer[0] = (char)(int)invalid_parsed1;
    buffer[1] = (char)(int)invalid_parsed2;
    buffer[2] = name_value && name_value[0] ? name_value[0] : '\0';
    buffer[3] = printed1 && printed1[0] ? printed1[0] : '\0';
    buffer[4] = (char)(int)name_is_str;
    (void)greeting_item;
    (void)parse_end;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[5] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}