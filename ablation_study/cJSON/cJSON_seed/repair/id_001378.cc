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
//<ID> 1378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON *initial_true = cJSON_AddTrueToObject(config, "enabled");
    double nums[] = {1.5, 2.5, 3.75};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(config, "thresholds", arr);
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(config, "enabled", replacement_true);

    // step 3: Operate and Validate
    cJSON *first_el = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_el);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out ? out[0] : '\0';
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)first_val % 10));
    buf[3] = (char)('0' + ((int)first_val / 10 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}