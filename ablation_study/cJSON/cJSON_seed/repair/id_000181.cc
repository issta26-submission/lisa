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
//<ID> 181
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
    cJSON_AddItemToObject(root, "data", data);
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(data, "metrics", metrics);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "n1", n1);
    cJSON *n2 = cJSON_CreateNumber(58.0);
    cJSON_AddItemToObject(metrics, "n2", n2);
    cJSON *raw = cJSON_AddRawToObject(metrics, "raw_list", "[100,200]");

    // step 3: Operate and Validate
    cJSON *n1_get = cJSON_GetObjectItem(metrics, "n1");
    cJSON *n2_get = cJSON_GetObjectItem(metrics, "n2");
    double v1 = cJSON_GetNumberValue(n1_get);
    double v2 = cJSON_GetNumberValue(n2_get);
    double sum = v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(metrics, "sum", sum_item);
    cJSON *raw_get = cJSON_GetObjectItem(metrics, "raw_list");
    cJSON *detached_raw = cJSON_DetachItemViaPointer(metrics, raw_get);
    cJSON_AddItemToObject(root, "detached_raw", detached_raw);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}