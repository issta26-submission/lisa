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
//<ID> 2317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *values = cJSON_AddArrayToObject(payload, "values");

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(values, cJSON_CreateString("thirty"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "format", cJSON_CreateString("v1"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *retrieved_payload = cJSON_GetObjectItem(root, "payload");
    cJSON *retrieved_values = cJSON_GetObjectItem(retrieved_payload, "values");
    int count = cJSON_GetArraySize(retrieved_values);
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = count > 0 ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}