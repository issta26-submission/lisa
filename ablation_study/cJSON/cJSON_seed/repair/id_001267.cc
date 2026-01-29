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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial = "{\"orig\":\"value\"}";
    cJSON *root = cJSON_Parse(initial);
    cJSON *created_str = cJSON_CreateString("created");
    cJSON_AddItemToObject(root, "new_str", created_str);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(nested, "inner_key", inner);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}