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
//<ID> 841
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num_field = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToObject(child, "value", num_field);
    double set_ret = cJSON_SetNumberHelper(num_field, 123.456);
    (void)set_ret;

    // step 3: Operate
    cJSON *new_num = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemInObjectCaseSensitive(child, "value", new_num);
    char raw_json[] = " { \"alpha\": 10, \"beta\": 20 } ";
    cJSON_Minify(raw_json);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON *got = cJSON_GetObjectItemCaseSensitive(child, "value");
    double got_val = cJSON_GetNumberValue(got);
    (void)got_val;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}