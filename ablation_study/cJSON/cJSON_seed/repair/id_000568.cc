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
//<ID> 568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *values = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "unit", cJSON_CreateString("meters"));

    // step 2: Configure
    cJSON *meta_obj = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *unit_item = cJSON_GetObjectItemCaseSensitive(meta_obj, "unit");
    char *unit_val = cJSON_GetStringValue(unit_item);
    cJSON_AddStringToObject(root, "unit_copy", unit_val);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(values, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}