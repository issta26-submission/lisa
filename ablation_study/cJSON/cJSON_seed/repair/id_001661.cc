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
//<ID> 1661
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nullval\":null,\"name\":\"parsed\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *local_str = cJSON_CreateString("local");
    cJSON_AddItemToObject(root, "local", local_str);

    // step 2: Configure
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON_AddItemReferenceToObject(root, "alias_parsed", parsed_name);

    // step 3: Operate and Validate
    cJSON *parsed_null = cJSON_GetObjectItem(parsed, "nullval");
    cJSON_bool is_null = cJSON_IsNull(parsed_null);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_null);
    const char *name_str = cJSON_GetStringValue(parsed_name);
    buffer[2] = name_str ? name_str[0] : 'Y';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66; // API sequence test completed successfully
}