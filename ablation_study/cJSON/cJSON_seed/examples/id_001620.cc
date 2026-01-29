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
//<ID> 1620
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
    cJSON *s = cJSON_CreateString("greeting");
    cJSON_AddItemToArray(arr, s);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);

    // step 2: Configure
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(arr, s);
    cJSON *num_ref_flag = cJSON_AddNumberToObject(root, "ref_added", (double)ref_added);
    cJSON_bool is_null = cJSON_IsNull(null_item);
    cJSON *num_null_flag = cJSON_AddNumberToObject(root, "null_is_null", (double)is_null);
    cJSON *num_size = cJSON_AddNumberToObject(root, "array_size", (double)cJSON_GetArraySize(arr));

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}