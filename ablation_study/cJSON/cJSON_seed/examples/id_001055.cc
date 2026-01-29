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
//<ID> 1055
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

    // step 2: Configure
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *falsy = cJSON_CreateFalse();
    cJSON_bool added_false = cJSON_AddItemToObject(root, "active", falsy);
    cJSON *added_null = cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *countNum = cJSON_CreateNumber((double)count);
    cJSON_bool added_count = cJSON_AddItemToObject(root, "count", countNum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}