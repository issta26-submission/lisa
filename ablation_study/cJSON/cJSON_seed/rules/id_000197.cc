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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_src[] = "{\"parsed\":\"yes\",\"value\":123}";
    size_t json_len = sizeof(json_src) - 1;
    const char *parse_end = NULL;
    cJSON_bool require_null = 1;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_src, json_len, &parse_end, require_null);
    cJSON *inner_parsed = cJSON_GetObjectItemCaseSensitive(parsed, "parsed");
    cJSON *fetched_inner = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "status", "initialized");
    cJSON_AddStringToObject(child, "note", "child created");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    fetched_inner = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(root, "imported"), "parsed");
    (void)inner_parsed;
    (void)fetched_inner;
    (void)parse_end;
    (void)require_null;
    (void)json_len;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}