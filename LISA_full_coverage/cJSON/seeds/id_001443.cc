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
//<ID> 1443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_AddObjectToObject(root, "obj");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(obj, "array", arr);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, ins);

    // step 3: Operate & Validate
    cJSON_bool ref_is_invalid = cJSON_IsInvalid(arr_ref);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    (void)ref_is_invalid;
    (void)arr_is_array;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    cJSON_PrintPreallocated(root, buffer, (int)(printed_len + 1), 0);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(arr_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}