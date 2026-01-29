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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *val = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(data, "value", val);
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *meta = cJSON_CreateString("meta-info");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *data_ptr = cJSON_GetObjectItem(root, "data");
    cJSON *value_ptr = cJSON_GetObjectItem(data_ptr, "value");

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(data_ptr, value_ptr, replacement);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}