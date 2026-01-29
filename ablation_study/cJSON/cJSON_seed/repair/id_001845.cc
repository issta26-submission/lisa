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
//<ID> 1845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToArray(data, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(data, cJSON_CreateString("three"));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "source", cJSON_CreateString("sensor"));
    cJSON_AddItemToObject(meta, "id", cJSON_CreateNumber(42.0));

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(data);
    cJSON *detached = cJSON_DetachItemFromArray(data, 1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached");
    double detached_val = cJSON_GetNumberValue(got_detached);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 16);
    memset(buffer, 0, printed_len + 16);
    memcpy(buffer, printed, printed_len);
    cJSON_Minify(buffer);
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    int size_after = cJSON_GetArraySize(got_data);
    (void)detached_val; (void)size_before; (void)size_after;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}