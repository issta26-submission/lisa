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
//<ID> 1665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"Alice\",\"age\":30,\"empty\":null}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *override_name = cJSON_CreateString("OverrideName");
    cJSON_AddItemToObject(root, "override", override_name);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "ref_name", parsed_name);

    // step 2: Configure
    cJSON *extra = cJSON_CreateString("extra_info");
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("item1"));

    // step 3: Operate and Validate
    cJSON *ref_item = cJSON_GetObjectItem(root, "ref_name");
    const char *ref_str = cJSON_GetStringValue(ref_item);
    cJSON *empty_item = cJSON_GetObjectItem(parsed, "empty");
    cJSON_bool is_empty = cJSON_IsNull(empty_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ref_str ? ref_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)is_empty);
    (void)parse_end;
    (void)ref_added;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}