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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"id\":123,\"name\":\"original\"}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON_bool require_null = 1;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, require_null);
    cJSON *root = cJSON_CreateObject();
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "payload", parsed);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");

    // step 3: Operate / Validate
    const char *name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_value);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}