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
//<ID> 156
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(child_array, num_item);
    cJSON_AddItemToObject(root, "original", child_array);

    // step 2: Configure
    cJSON *array_ref = cJSON_CreateArrayReference(child_array);
    cJSON_AddItemReferenceToObject(root, "reference", array_ref);
    const int buf_len = 256;
    char *prebuf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prebuf, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, prebuf, buf_len, 1);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(child_array);
    cJSON_bool compare_equal = cJSON_Compare(array_ref, child_array, 1);
    cJSON *first = cJSON_GetArrayItem(child_array, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    double third_val = cJSON_GetNumberValue(cJSON_GetArrayItem(child_array, 2));
    int summary = size + (int)compare_equal + (int)first_is_true + (int)third_val + (int)printed_ok + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}