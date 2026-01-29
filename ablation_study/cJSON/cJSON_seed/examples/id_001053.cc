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
//<ID> 1053
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok_flag", flag_false);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON *it2 = cJSON_GetArrayItem(arr, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    double sum = v0 + v1 + v2;
    cJSON *sumNum = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sumNum);
    cJSON *countNum = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", countNum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}