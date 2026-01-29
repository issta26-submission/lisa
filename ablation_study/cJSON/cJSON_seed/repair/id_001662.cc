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
//<ID> 1662
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"key\":\"value\",\"nested\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *ref_str = cJSON_CreateString("ref_value");
    cJSON_bool added = cJSON_AddItemReferenceToObject(root, "ref", ref_str);

    // step 2: Configure
    cJSON *plain = cJSON_CreateString("plain_value");
    cJSON_AddItemToObject(root, "plain", plain);

    // step 3: Operate and Validate
    cJSON *key_item = cJSON_GetObjectItem(root, "key");
    const char *key_val = cJSON_GetStringValue(key_item);
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON_bool is_null = cJSON_IsNull(nested_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = key_val ? key_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)is_null);
    (void)added;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}