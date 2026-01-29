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
//<ID> 1052
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    int size = cJSON_GetArraySize(arr);
    cJSON *countNum = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", countNum);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *vals = cJSON_GetObjectItem(root, "values");
    int size_copy = cJSON_GetArraySize(vals);
    cJSON *countCopy = cJSON_CreateNumber((double)size_copy);
    cJSON_AddItemToObject(root, "count_copy", countCopy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}