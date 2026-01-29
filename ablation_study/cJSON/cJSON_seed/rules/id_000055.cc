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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"value\":42,\"nested\":{\"key\":\"old\"}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *nested = cJSON_GetObjectItemCaseSensitive(root, "nested");
    cJSON *old_key = cJSON_GetObjectItemCaseSensitive(nested, "key");

    // step 2: Setup / Configure
    cJSON *new_raw = cJSON_CreateRaw("\"new\"");
    cJSON *raw_number = cJSON_CreateRaw("123");
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(nested, old_key, new_raw);
    cJSON_bool add_result = cJSON_AddItemToObject(root, "raw_number", raw_number);

    // step 3: Operate / Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *name_value = cJSON_GetStringValue(name_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)name_value;
    (void)replace_result;
    (void)add_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}