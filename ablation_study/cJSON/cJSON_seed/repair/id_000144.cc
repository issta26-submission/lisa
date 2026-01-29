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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "initial");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    double active_flag = (double)cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    double scaled_sum = sum * active_flag;
    cJSON_AddNumberToObject(root, "scaled_sum", scaled_sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}