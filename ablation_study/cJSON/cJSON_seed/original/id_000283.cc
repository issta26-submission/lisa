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
//<ID> 283
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *flag = cJSON_CreateFalse();
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "flag", flag);
    cJSON_AddItemToArray(arr, obj);

    // step 2: Configure
    cJSON *array_ref = cJSON_CreateArrayReference(obj);
    cJSON_AddItemToObject(root, "obj_ref_array", array_ref);
    cJSON *num2 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(array_ref, num2);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *ref_first = cJSON_GetArrayItem(array_ref, 0);
    cJSON *flag_item = cJSON_GetObjectItem(second, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    (void)ref_first;
    (void)flag_is_bool;
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}