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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json = "{\"replace_me\":100}";
    size_t json_len = 18;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement_raw = NULL;
    cJSON *parsed_number = NULL;
    cJSON *new_item = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure structure
    raw_item = cJSON_CreateRaw("777");
    cJSON_AddItemToObject(root, "rawValue", raw_item);
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 0);
    cJSON_AddItemToObject(root, "parsed", parsed);
    parsed_number = cJSON_GetObjectItemCaseSensitive(parsed, "replace_me");

    // step 3: Operate / Validate
    replacement_raw = cJSON_CreateRaw("999");
    cJSON_ReplaceItemViaPointer(parsed, parsed_number, replacement_raw);
    new_item = cJSON_GetObjectItemCaseSensitive(parsed, "replace_me");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}