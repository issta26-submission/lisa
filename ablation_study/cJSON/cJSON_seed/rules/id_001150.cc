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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *dup_parsed = NULL;
    cJSON *name_item = NULL;
    cJSON *null_item = NULL;
    cJSON *embedded = NULL;
    char *printed = NULL;
    const char *parse_end = NULL;
    char json[] = "{\"name\": \"example\", \"value\": 42, \"nested\": {\"a\": 1}}";
    size_t json_len = (size_t)(sizeof(json) - 1);

    // step 2: Setup / Configure
    cJSON_Minify(json);
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    dup_parsed = cJSON_Duplicate(parsed, 1);
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("root_name");
    cJSON_AddItemToObject(root, "name", name_item);
    null_item = cJSON_AddNullToObject(root, "no_value");
    cJSON_AddItemToObject(root, "embedded", dup_parsed);

    // step 3: Operate / Validate
    embedded = cJSON_GetObjectItem(root, "embedded");
    (void)cJSON_GetNumberValue(cJSON_GetObjectItem(embedded, "value"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}