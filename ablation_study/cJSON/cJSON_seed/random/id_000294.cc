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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *got_container = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    char raw_text[] = "{\"embedded\":true}";
    char greet[] = "hello cJSON";

    // step 2: Initialize
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw(raw_text);

    // step 3: Configure
    cJSON_AddStringToObject(root, "greeting", greet);
    cJSON_AddStringToObject(nested, "inner_key", "inner_value");
    cJSON_AddItemToObject(nested, "raw", raw_item);
    cJSON_AddItemToObject(root, "container", nested);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 4: Operate
    got_container = cJSON_GetObjectItem(root, "container");
    got_raw = cJSON_GetObjectItem(got_container, "raw");
    got_arr = cJSON_GetObjectItem(root, "my_array");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "container_has_array", is_arr);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}