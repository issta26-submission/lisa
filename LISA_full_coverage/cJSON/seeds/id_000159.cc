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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateArray();
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON *b_true2 = cJSON_CreateBool(1);
    cJSON_AddItemToArray(child, b_true);
    cJSON_AddItemToArray(child, b_false);
    cJSON_AddItemToArray(child, b_true2);
    cJSON_AddItemToObject(root, "actual_booleans", child);

    // step 2: Configure
    cJSON *array_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemReferenceToObject(root, "alias_booleans", array_ref);
    cJSON *feature_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "feature_enabled", feature_flag);

    // step 3: Operate and Validate
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int array_size = cJSON_GetArraySize(child);
    cJSON *first_item = cJSON_GetArrayItem(child, 0);
    cJSON *second_item = cJSON_GetArrayItem(child, 1);
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    cJSON_bool second_is_false = cJSON_IsFalse(second_item);
    int summary = array_size + (int)first_is_true + (int)second_is_false + root->type + feature_flag->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}