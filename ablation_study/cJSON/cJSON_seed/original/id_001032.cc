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
//<ID> 1032
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"id\":101,\"name\":\"original\",\"payload\":\"to_be_replaced\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *new_name = cJSON_SetValuestring(name_item, "modified_name");
    cJSON *out = cJSON_CreateObject();
    cJSON_AddNumberToObject(out, "original_id", cJSON_GetNumberValue(id_item));
    cJSON_AddStringToObject(out, "modified_name", new_name);
    cJSON *raw_payload = cJSON_CreateRaw("{\"raw_key\":42}");
    cJSON_AddItemToObject(out, "payload_raw", raw_payload);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_payload);
    cJSON_AddBoolToObject(out, "payload_was_raw", raw_flag);

    // step 3: Operate
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