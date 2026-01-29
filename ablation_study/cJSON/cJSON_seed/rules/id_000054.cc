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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    const char json[] = "{\"name\":\"example\",\"replace_me\":{\"val\":1},\"raw\":\"<data>\"}";
    const char *parse_end = NULL;
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *replacement = cJSON_CreateObject();
    cJSON *replacement_num = cJSON_CreateNumber(42.0);
    cJSON *target = NULL;
    cJSON *found_after = NULL;
    char *printed = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool replace_ok = 0;

    // step 2: Setup
    add_ok1 = cJSON_AddItemToObject(root, "parsed", parsed);
    add_ok2 = cJSON_AddItemToObject(parsed, "raw_added", raw_item);
    cJSON_AddItemToObject(replacement, "newval", replacement_num);

    // step 3: Operate / Validate
    target = cJSON_GetObjectItemCaseSensitive(parsed, "replace_me");
    replace_ok = cJSON_ReplaceItemViaPointer(parsed, target, replacement);
    found_after = cJSON_GetObjectItemCaseSensitive(parsed, "replace_me");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}