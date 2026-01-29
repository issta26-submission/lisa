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
//<ID> 1370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_false);
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", num_array);

    // step 2: Configure
    cJSON *explicit_true = cJSON_AddTrueToObject(root, "explicit_true");
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "enabled", new_true);

    // step 3: Operate and Validate
    cJSON *first_num_item = cJSON_GetArrayItem(num_array, 0);
    double first_val = cJSON_GetNumberValue(first_num_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)first_val % 10));
    buf[3] = (char)('0' + (explicit_true ? (explicit_true->type % 10) : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}