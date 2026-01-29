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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "label", "sample");

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(4.4));
    cJSON_DeleteItemFromArray(values, 1);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *picked = cJSON_GetArrayItem(got_values, 1);
    double picked_val = cJSON_GetNumberValue(picked);
    cJSON_AddNumberToObject(root, "picked", picked_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}