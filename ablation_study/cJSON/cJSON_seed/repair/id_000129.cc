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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"source\":\"from_parse\",\"count\":3}");
    cJSON *src_item = cJSON_GetObjectItem(parsed, "source");
    const char *src = cJSON_GetStringValue(src_item);
    cJSON_AddStringToObject(root, "source_copy", src);

    // step 2: Configure
    cJSON *placeholder = cJSON_AddArrayToObject(root, "values");
    double nums[4] = {1.5, 2.5, 3.5, 4.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 4);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, placeholder, double_arr);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}