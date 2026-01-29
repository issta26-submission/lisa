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
//<ID> 1137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *label = cJSON_CreateString("demo_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(dbl_array, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool enabled = cJSON_IsTrue(flag);
    double result = first_val + (double)enabled;
    cJSON_AddNumberToObject(root, "computed", result);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}