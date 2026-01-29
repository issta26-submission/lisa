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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *s = cJSON_CreateString("created-string");
    cJSON_AddItemToObject(nested, "str", s);
    cJSON *arr = cJSON_CreateArray();
    cJSON *a1 = cJSON_CreateString("elem1");
    cJSON *a2 = cJSON_CreateString("elem2");
    cJSON_AddItemToArray(arr, a1);
    cJSON_AddItemToArray(arr, a2);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size_num", size_num);

    // step 3: Operate and Validate
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