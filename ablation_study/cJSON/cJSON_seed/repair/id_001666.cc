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
//<ID> 1666
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"key\":\"value\",\"null_item\":null}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToObject(root, "greeting", str_item);
    cJSON *parsed_null = cJSON_GetObjectItem(parsed, "null_item");
    cJSON_bool is_null = cJSON_IsNull(parsed_null);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)add_ref_ok);
    buffer[2] = (char)('0' + (int)is_null);
    const char *greeting = cJSON_GetStringValue(str_item);
    buffer[3] = greeting ? greeting[0] : 'Y';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}