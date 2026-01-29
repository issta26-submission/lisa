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
//<ID> 1372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.75};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "threshold", 10.0);
    cJSON *auto_flag = cJSON_AddTrueToObject(root, "auto_enabled");

    // step 2: Configure
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "threshold", new_true);
    cJSON *values_obj = cJSON_GetObjectItem(root, "values");
    cJSON *second_item = cJSON_GetArrayItem(values_obj, 1);
    double second_val = cJSON_GetNumberValue(second_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out ? out[0] : 0;
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)second_val % 10));
    buf[3] = (char)('0' + ((int)second_val / 10 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}