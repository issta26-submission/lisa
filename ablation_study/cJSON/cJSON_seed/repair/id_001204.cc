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
//<ID> 1204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(data, "name", str_item);
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(data, "enabled", bool_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(data, "count", num_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(data, "optional", null_item);
    cJSON_bool is_null = cJSON_IsNull(null_item);

    // step 2: Configure
    double num_val = cJSON_GetNumberValue(num_item);
    double final_val = num_val + (double)is_null;
    cJSON *final_num_item = cJSON_CreateNumber(final_val);
    cJSON_AddItemToObject(root, "final", final_num_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    const char *name_val = cJSON_GetStringValue(str_item);
    ((char *)scratch)[2] = name_val ? name_val[0] : '\0';

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}