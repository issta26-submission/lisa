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
//<ID> 1374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[3] = {1.5, 2.5, 3.5};
    cJSON *vals = cJSON_CreateDoubleArray(nums, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", vals);
    cJSON_AddTrueToObject(root, "flag");
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "flag", new_true);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(vals, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)second_val % 10));
    buf[3] = out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}