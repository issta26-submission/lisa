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
//<ID> 1225
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, s);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(2.25);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *tag = cJSON_CreateString("v1");
    cJSON_AddItemToObject(root, "tag", tag);
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON *it2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_str = cJSON_IsString(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    double total = v1 + v2 + (double)is_str;
    cJSON_AddNumberToObject(root, "total", total);
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