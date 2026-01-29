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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[10,20,30],\"name\":\"orig\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *extra = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}