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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_AddStringToObject(root, "name", "tester");
    cJSON *optional = cJSON_AddNullToObject(root, "optional");
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "count", num_item);

    // step 2: Configure
    const char *json_text = "{\"flag\":true, \"value\":42}";
    size_t json_len = sizeof("{\"flag\":true, \"value\":42}") - 1;
    const char *endptr = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &endptr, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double set_ret = cJSON_SetNumberHelper(num_item, 7.5);
    cJSON *detached_val = cJSON_DetachItemViaPointer(parsed, cJSON_GetObjectItem(parsed, "value"));
    cJSON_AddItemToObject(root, "imported_value", detached_val);
    double imported_number = cJSON_GetNumberValue(detached_val);
    double final_value = set_ret + imported_number;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}