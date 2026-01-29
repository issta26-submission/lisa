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
//<ID> 188
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", answer);
    cJSON *raw_item = cJSON_AddRawToObject(meta, "raw_payload", "{\"flag\":true}");

    // step 3: Operate and Validate
    cJSON *meta_lookup = cJSON_GetObjectItem(root, "meta");
    cJSON *answer_lookup = cJSON_GetObjectItem(meta_lookup, "answer");
    double base_value = cJSON_GetNumberValue(answer_lookup);
    cJSON *incremented = cJSON_CreateNumber(base_value + 1.0);
    cJSON_AddItemToArray(list, incremented);
    cJSON *detached = cJSON_DetachItemViaPointer(meta_lookup, answer_lookup);
    cJSON_AddItemToArray(list, detached);
    cJSON *raw_lookup = cJSON_GetObjectItem(meta_lookup, "raw_payload");
    char *raw_str = cJSON_GetStringValue(raw_lookup);
    double augment = 2.0 * (raw_str != NULL);
    cJSON *augnum = cJSON_CreateNumber(base_value + augment);
    cJSON_AddItemToArray(list, augnum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}