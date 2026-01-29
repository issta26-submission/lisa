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
//<ID> 159
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
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "flags", flags);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(flags, "active", active);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(items, n0);
    cJSON *n1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(items, n1);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    double sum = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}