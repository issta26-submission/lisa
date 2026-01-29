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
//<ID> 1322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", "Example");
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *values_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values_arr);

    // step 2: Configure
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool retrieved_is_array = cJSON_IsArray(retrieved_values);
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    const char *title_str = cJSON_GetStringValue(title_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    int scratch_size = 128;
    void *scratch = cJSON_malloc((size_t)scratch_size);
    memset(scratch, 0, (size_t)scratch_size);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (retrieved_is_array ? 1 : 0));
    ((char *)scratch)[2] = title_str ? title_str[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}