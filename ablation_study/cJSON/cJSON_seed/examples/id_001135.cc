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
//<ID> 1135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[] = {1.5, 2.5, 3.5};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *label = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_arr);
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *item0 = cJSON_GetArrayItem(dbl_arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(dbl_arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    cJSON_bool is_enabled = cJSON_IsTrue(flag);
    double total = v0 + v1 + (double)is_enabled;
    cJSON *sum = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}