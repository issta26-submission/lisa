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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"value\":42,\"data\":\"old\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *old_item = cJSON_CreateString("to_be_replaced");
    cJSON *raw_replacement = cJSON_CreateRaw("123");
    cJSON *copied_data = NULL;
    cJSON *found = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "parsed", parsed);
    found = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    copied_data = cJSON_CreateString(cJSON_GetStringValue(found));
    cJSON_AddItemToObject(root, "copied_data", copied_data);
    cJSON_AddItemToObject(root, "replace_me", old_item);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, raw_replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}