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
//<ID> 1233
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_ref = cJSON_CreateStringReference("example");
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, bool_false);

    // step 2: Configure
    cJSON *it_num = cJSON_GetArrayItem(arr, 0);
    cJSON *it_ref = cJSON_GetArrayItem(arr, 1);
    cJSON *it_bool = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_bool = cJSON_IsBool(it_bool);
    cJSON_bool is_false = cJSON_IsFalse(it_bool);

    // step 3: Operate and Validate
    double base_val = cJSON_GetNumberValue(it_num);
    double bool_contrib = (double)(is_bool + is_false);
    double combined = base_val + bool_contrib;
    cJSON *computed = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}