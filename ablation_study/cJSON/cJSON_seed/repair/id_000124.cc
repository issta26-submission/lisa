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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *wrapper = cJSON_AddArrayToObject(root, "array_wrapper");
    double nums[3] = {1.5, 2.5, 3.25};
    cJSON *dblArr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToArray(wrapper, dblArr);
    cJSON_AddStringToObject(root, "label", "example_label");
    cJSON *oldnum = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "replace_me", oldnum);

    // step 2: Configure
    cJSON *parsed = cJSON_Parse("99.5");
    cJSON_ReplaceItemViaPointer(root, oldnum, parsed);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItem(root, "replace_me");
    double replaced_val = cJSON_GetNumberValue(got);
    int arr_size = cJSON_GetArraySize(dblArr);
    cJSON *ai0 = cJSON_GetArrayItem(dblArr, 0);
    cJSON *ai1 = cJSON_GetArrayItem(dblArr, 1);
    cJSON *ai2 = cJSON_GetArrayItem(dblArr, 2);
    double sum = cJSON_GetNumberValue(ai0) + cJSON_GetNumberValue(ai1) + cJSON_GetNumberValue(ai2) + replaced_val;
    (void)arr_size;
    (void)sum;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}