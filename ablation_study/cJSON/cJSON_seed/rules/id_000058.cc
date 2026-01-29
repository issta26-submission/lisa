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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char raw_text[] = "{\"inner\":42}";
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw(raw_text);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *found = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double inner_val = 0.0;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup
    added = cJSON_AddItemToObject(root, "raw_payload", raw);
    parsed = cJSON_ParseWithLengthOpts(raw_text, (size_t)(sizeof(raw_text) - 1), &parse_end, 1);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItemCaseSensitive(parsed, "inner");
    inner_val = cJSON_GetNumberValue(found);
    replacement = cJSON_CreateNumber(inner_val * 2.0);
    replaced = cJSON_ReplaceItemViaPointer(root, raw, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    return 66;
}
// API sequence test completed successfully