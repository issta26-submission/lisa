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
//<ID> 1044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *sref = cJSON_CreateStringReference("example-ref");
    cJSON_AddItemToObject(root, "ref", sref);
    cJSON_AddItemToObject(root, "nums", arr);

    // step 2: Configure
    const char *json1 = "{\"alpha\":3.0,\"beta\":4.0}";
    size_t len1 = sizeof("{\"alpha\":3.0,\"beta\":4.0}") - 1;
    const char *endptr1 = NULL;
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(json1, len1, &endptr1, 1);
    cJSON *parsed2 = cJSON_Parse("[10,20,30]");
    cJSON_bool parsed2_is_array = cJSON_IsArray(parsed2);
    cJSON_AddNumberToObject(root, "parsed2_is_array", (double)parsed2_is_array);

    // step 3: Operate & Validate
    cJSON *a_item = cJSON_GetObjectItem(parsed1, "alpha");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON_AddNumberToObject(root, "alpha_val", a_val);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON *sum_num = cJSON_CreateNumber(a_val + (double)arr_size);
    cJSON_AddItemToObject(root, "sum", sum_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed2);
    cJSON_Delete(parsed1);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}