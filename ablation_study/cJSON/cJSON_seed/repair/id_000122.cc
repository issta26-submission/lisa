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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title_item = cJSON_AddStringToObject(root, "title", "cJSON double array test");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    const double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToArray(values, dbl_arr);

    // step 2: Configure
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "initialized");
    cJSON *parsed = cJSON_Parse("{\"replaced\":true,\"note\":\"parsed\"}");

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(values, dbl_arr, parsed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}