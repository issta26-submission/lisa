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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":42}");

    // step 2: Configure
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddItemToObject(root, "payload", raw_item);
    cJSON *values_array = cJSON_AddArrayToObject(root, "values");
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON *v3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values_array, v1);
    cJSON_AddItemToArray(values_array, v2);
    cJSON_AddItemToArray(values_array, v3);

    // step 3: Operate
    int count = cJSON_GetArraySize(values_array);
    cJSON *summary = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(summary, "count", count_item);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}