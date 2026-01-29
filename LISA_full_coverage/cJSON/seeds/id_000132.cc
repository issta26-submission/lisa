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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    float nums[] = { 1.5f, 2.5f, 3.5f };
    cJSON *nums_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON_AddItemToObject(root, "values", nums_arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "creator", "synth");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    cJSON_bool equal = cJSON_Compare(root, root_dup, 1);
    char *printed = cJSON_PrintUnformatted(root);
    int names_count = cJSON_GetArraySize(names_arr);
    int values_count = cJSON_GetArraySize(nums_arr);
    int summary = names_count + values_count + (int)equal + (printed ? (int)printed[0] : 0) + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(root_dup);

    // API sequence test completed successfully
    return 66;
}