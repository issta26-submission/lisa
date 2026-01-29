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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNullToObject(meta, "nullable");
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *extra = cJSON_AddObjectToObject(root, "extra");
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(extra, "status", status);
    cJSON *inner = cJSON_CreateArray();
    cJSON_AddItemToArray(inner, cJSON_CreateNumber(1.0));
    cJSON_AddItemToObject(extra, "inner", inner);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON_bool is_array = cJSON_IsArray(got_values);
    cJSON_AddItemToArray(got_values, cJSON_CreateNumber(99.9));
    cJSON *flag = cJSON_CreateBool(is_array);
    cJSON_AddItemToObject(root, "values_is_array", flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}