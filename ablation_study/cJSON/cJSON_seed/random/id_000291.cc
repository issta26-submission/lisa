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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_raw = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw = cJSON_CreateRaw("embedded_raw_payload");

    // step 3: Configure
    greeting = cJSON_AddStringToObject(root, "greeting", "hello world");
    add_ok_arr = cJSON_AddItemToObject(root, "my_array", arr);
    add_ok_raw = cJSON_AddItemToObject(root, "blob", raw);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "my_array");
    got_raw = cJSON_GetObjectItem(root, "blob");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    cJSON_AddStringToObject(root, "my_array_is", is_arr ? "array" : "not_array");
    cJSON_AddStringToObject(root, "blob_is", is_raw ? "raw" : "not_raw");
    cJSON_AddStringToObject(root, "arr_add_ok", add_ok_arr ? "ok" : "fail");
    cJSON_AddStringToObject(root, "raw_add_ok", add_ok_raw ? "ok" : "fail");

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}