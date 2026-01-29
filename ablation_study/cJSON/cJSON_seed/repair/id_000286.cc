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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\":[1.5,2.5],\"flag\":true}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    double extra_nums[2] = {3.0, 4.25};
    cJSON *new_array = cJSON_CreateDoubleArray(extra_nums, 2);
    cJSON_AddFalseToObject(root, "added_false");

    // step 3: Operate and Validate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON_AddItemToArray(values, new_array);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    double flag_is_true = (double)cJSON_IsTrue(flag_item);
    cJSON_AddNumberToObject(root, "flag_numeric", flag_is_true);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}