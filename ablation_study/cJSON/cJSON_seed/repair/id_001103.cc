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
//<ID> 1103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *bf = cJSON_AddFalseToObject(child, "enabled");
    cJSON *array_ref = cJSON_CreateArrayReference(child);
    cJSON_bool added_ref = cJSON_AddItemToObject(root, "ref_array", array_ref);
    cJSON *sub = cJSON_AddObjectToObject(root, "sub");

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(sub, "value", num);
    cJSON_AddItemToObject(sub, "child", child);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(bf);
    cJSON_bool is_false = cJSON_IsFalse(bf);
    int arr_size = cJSON_GetArraySize(array_ref);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (int)is_bool);
    ((char *)scratch)[2] = (char)('0' + (int)is_false);
    ((char *)scratch)[3] = (char)('0' + (arr_size & 0xF));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}