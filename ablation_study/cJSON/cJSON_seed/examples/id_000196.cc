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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", array);

    // step 2: Configure
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\": 123}");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddItemToArray(array, raw_item);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(array, str_item);
    cJSON *meta_str = cJSON_CreateString("generated_by_test");
    cJSON_AddItemToObject(root, "meta", meta_str);

    // step 3: Operate & Validate
    cJSON *status = cJSON_CreateString("raw_checked");
    (void)raw_flag;
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}