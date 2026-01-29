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
//<ID> 1669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "null";
    const char *parse_end = NULL;
    cJSON *parsed_null = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("embedded");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "greeting", str_item);

    // step 3: Operate and Validate
    cJSON_bool parsed_is_null = cJSON_IsNull(parsed_null);
    cJSON *greeting_item = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(greeting_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)parsed_is_null);
    buffer[2] = greet_val ? greet_val[0] : 'Y';
    (void)ref_added;
    (void)parse_end;
    (void)greeting_item;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed_null);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}