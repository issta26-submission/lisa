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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.5,2.5,3.5],\"num\":10,\"info\":\"test\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *active = cJSON_AddTrueToObject(root, "active");
    double replacement_vals[] = {9.9, 8.8, 7.7};
    cJSON *new_arr = cJSON_CreateDoubleArray(replacement_vals, 3);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", new_arr);
    cJSON *new_num = cJSON_CreateNumber(42.42);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "num", new_num);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}