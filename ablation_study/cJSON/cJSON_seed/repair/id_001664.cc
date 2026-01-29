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
//<ID> 1664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"parsedName\",\"value\":7}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *created_str = cJSON_CreateString("referenced_str");
    cJSON_AddItemToObject(root, "orig", created_str);
    cJSON *alias_container = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(alias_container, "alias", created_str);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(alias_container, "maybe_null", maybe_null);

    // step 3: Operate and Validate
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    char *printed = cJSON_PrintUnformatted(alias_container);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)is_null);
    const char *sval = cJSON_GetStringValue(created_str);
    buffer[1] = sval ? sval[0] : 'X';
    buffer[2] = printed ? printed[0] : 'X';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(alias_container);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66; // API sequence test completed successfully
}