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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"alpha\",\"nested\":{\"num\":3}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "alpha_modified");
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "copied_name", cJSON_CreateString(cJSON_GetStringValue(name_item)));
    cJSON *raw_item = cJSON_CreateRaw("{\"injected\":true}");
    cJSON_AddItemToObject(out, "raw_in", raw_item);

    // step 3: Operate
    cJSON_AddBoolToObject(out, "raw_was_raw", cJSON_IsRaw(raw_item));
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}