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
//<ID> 293
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
    cJSON *addstr_ret = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON *got_status = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *dup_raw = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_raw = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw = cJSON_CreateRaw("raw-data");

    // step 3: Configure
    add_ok_arr = cJSON_AddItemToObject(root, "data", arr);
    add_ok_raw = cJSON_AddItemToObject(root, "rawval", raw);
    addstr_ret = cJSON_AddStringToObject(root, "status", "ready");

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "data");
    got_raw = cJSON_GetObjectItem(root, "rawval");
    got_status = cJSON_GetObjectItem(root, "status");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    dup_arr = cJSON_Duplicate(got_arr, 1);
    cJSON_AddItemToObject(root, "got_array_copy", dup_arr);
    dup_raw = cJSON_Duplicate(got_raw, 1);
    cJSON_AddItemToObject(root, "got_raw_copy", dup_raw);
    cJSON_AddStringToObject(root, "array_check", is_arr ? "true" : "false");
    cJSON_AddStringToObject(root, "raw_check", is_raw ? "true" : "false");
    (void)got_status; // used to show retrieval step; value present in root

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}