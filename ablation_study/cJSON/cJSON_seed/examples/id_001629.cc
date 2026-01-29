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
//<ID> 1629
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
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "items_count", (double)arr_size);
    cJSON_AddNumberToObject(root, "constant_pi", 3.14159265);

    // step 3: Operate & Validate
    cJSON_AddItemReferenceToArray(arr, s1);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "maybe_null_flag", (double)is_null_flag);

    // step 4: Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}