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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[3] = {1.1, 2.2, 3.3};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *orig_count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(meta, "orig_count", orig_count);

    // step 2: Configure
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    double first = cJSON_GetNumberValue(cJSON_GetArrayItem(got_values, 0));
    double last = cJSON_GetNumberValue(cJSON_GetArrayItem(got_values, 2));
    double sum = first + last;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum_endpoints", sum_item);
    cJSON_SetNumberHelper(orig_count, 2.0);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(got_values, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}