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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"root\":{\"list\":[1,2,3]},\"flag\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *list = cJSON_GetObjectItem(root_obj, "list");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");

    // step 2: Configure
    cJSON_bool list_is_array = cJSON_IsArray(list);
    cJSON *null_item = cJSON_AddNullToObject(root_obj, "maybe_null");
    cJSON *false_item = cJSON_AddFalseToObject(root_obj, "new_false");
    cJSON *array_flag_num = cJSON_AddNumberToObject(root_obj, "array_flag_val", (double)list_is_array * 100.0);
    size_t buf_len = 16 + (size_t)list_is_array * 16;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    double num_val = cJSON_GetNumberValue(array_flag_num);
    double combined = num_val + (double)flag_is_bool;
    (void)combined;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}