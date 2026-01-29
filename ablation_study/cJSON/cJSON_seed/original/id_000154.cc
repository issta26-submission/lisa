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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child_array = cJSON_CreateArray();
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(child_array, b_true);
    cJSON_AddItemToArray(child_array, b_false);

    // step 2: Configure
    char *key = (char*)cJSON_malloc(12);
    memcpy(key, "arr_ref_key", 12);
    cJSON *arr_ref = cJSON_CreateArrayReference(child_array);
    cJSON_AddItemReferenceToObject(root, key, arr_ref);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(child_array);
    cJSON *first = cJSON_GetArrayItem(child_array, 0);
    cJSON *second = cJSON_GetArrayItem(child_array, 1);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_bool second_is_false = cJSON_IsFalse(second);
    cJSON_bool same = cJSON_Compare(arr_ref, child_array, 1);
    char *out = cJSON_PrintUnformatted(root);
    int summary = size + (int)first_is_true + (int)second_is_false + (int)same + (out ? (int)out[0] : 0) + root->type + child_array->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(child_array);
    cJSON_free(key);

    // API sequence test completed successfully
    return 66;
}