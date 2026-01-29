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
//<ID> 865
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
    float numbers[3] = { 1.25f, 2.5f, 3.75f };
    cJSON *farr = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    const char json_buf[] = " { \"external\": { \"x\": 42.0, \"note\": \"ok\" } } ";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *external = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", external_ref);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    double external_x = cJSON_GetNumberValue(cJSON_GetObjectItem(external, "x"));
    int float_count = cJSON_GetArraySize(farr);
    (void)external_x;
    (void)float_count;

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}