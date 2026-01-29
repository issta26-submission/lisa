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
//<ID> 1325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label_item = cJSON_AddStringToObject(root, "label", "example_label");
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);

    // step 2: Configure
    cJSON *found_label = cJSON_GetObjectItem(root, "label");
    cJSON *found_doubles = cJSON_GetObjectItem(root, "doubles");
    cJSON_bool is_array = cJSON_IsArray(found_doubles);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);

    // step 3: Operate and Validate
    const char *label_str = cJSON_GetStringValue(found_label);
    ((char *)scratch)[0] = label_str ? label_str[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (is_array ? 1 : 0));
    ((char *)scratch)[2] = out ? out[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}