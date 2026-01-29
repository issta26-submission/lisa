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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-42");
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON_AddItemToObjectCS(root, "ints", int_array);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "1.0");

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_ints = cJSON_GetObjectItem(root, "ints");
    int arr_size = cJSON_GetArraySize(got_ints);
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON *agg = cJSON_CreateNumber((double)arr_size + (double)has_ints);
    cJSON_AddItemToObjectCS(root, "count_plus_flag", agg);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);
    char *copied = cJSON_GetStringValue(name_copy);
    (void)copied;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}