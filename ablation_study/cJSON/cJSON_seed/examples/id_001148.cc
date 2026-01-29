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
//<ID> 1148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_arr1 = "[10,20,30]";
    size_t json_arr1_len = 10;
    cJSON *arr1 = cJSON_ParseWithLength(json_arr1, json_arr1_len);
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_AddItemToObject(root, "arr1", arr1);
    const char *json_arr2 = "[4,5]";
    cJSON *arr2 = cJSON_Parse(json_arr2);
    cJSON_AddItemToObject(root, "arr2", arr2);

    // step 3: Operate & Validate
    cJSON *a0 = cJSON_GetArrayItem(arr1, 0);
    cJSON *b1 = cJSON_GetArrayItem(arr2, 1);
    double va0 = cJSON_GetNumberValue(a0);
    double vb1 = cJSON_GetNumberValue(b1);
    cJSON *sum = cJSON_CreateNumber(va0 + vb1);
    cJSON_AddItemToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}