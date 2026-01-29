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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *threshold_item = cJSON_AddNumberToObject(root, "threshold", 42.5);
    cJSON_AddNumberToObject(root, "pi", 3.14159265);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "build", 7);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "threshold");
    double detached_value = cJSON_GetNumberValue(detached);
    cJSON *orig_threshold = cJSON_GetObjectItem(root, "threshold");
    double orig_value = cJSON_GetNumberValue(orig_threshold);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig=%.2f;detached=%.2f",
            version ? version : "null",
            orig_value,
            detached_value);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}