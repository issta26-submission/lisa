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
//<ID> 1245
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.5);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "count", 2.0);
    double arr_flag = (double)cJSON_IsArray(arr);
    cJSON_AddNumberToObject(root, "is_array_flag", arr_flag);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_arr = cJSON_GetObjectItem(dup, "arr");
    double dup_arr_flag = (double)cJSON_IsArray(dup_arr);
    cJSON_AddNumberToObject(dup, "dup_is_array_flag", dup_arr_flag);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}