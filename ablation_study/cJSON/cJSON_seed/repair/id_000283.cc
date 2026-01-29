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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *false_item = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(arr, extra_num);
    cJSON_AddNumberToObject(root, "extra", 9.0);

    // step 3: Operate and Validate
    cJSON_bool active_true = cJSON_IsTrue(false_item);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_true);
    char *out = cJSON_PrintUnformatted(root);
    char buf[256];
    memset(buf, 0, sizeof(buf));
    cJSON_PrintPreallocated(root, buf, (int)sizeof(buf), 0);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}