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
//<ID> 545
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

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddNumberToObject(root, "count", 42.0);
    int numbers[4] = {1, 2, 3, 4};
    cJSON *ints_arr = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints_arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "build", 7.0);

    // step 3: Operate & Validate
    cJSON *detached_count = cJSON_DetachItemFromObjectCaseSensitive(root, "count");
    double detached_count_val = cJSON_GetNumberValue(detached_count);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int parsed_ints_size = cJSON_GetArraySize(parsed_ints);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.5f;count=%.0f;arrsize=%d",
            version ? version : "null",
            pi_val,
            detached_count_val,
            parsed_ints_size);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_count);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}