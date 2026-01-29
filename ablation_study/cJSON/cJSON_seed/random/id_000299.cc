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
//<ID> 299
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
    cJSON *raw_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    const char *arr_state = (const char *)0;
    const char *raw_state = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("raw_payload");

    // step 3: Configure
    cJSON_AddItemToObject(root, "myArray", arr);
    cJSON_AddItemToObject(root, "myRaw", raw_item);
    cJSON_AddStringToObject(root, "note", "creation_complete");

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "myArray");
    got_raw = cJSON_GetObjectItem(root, "myRaw");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    arr_state = is_arr ? "true" : "false";
    raw_state = is_raw ? "true" : "false";
    cJSON_AddStringToObject(root, "array_is_array", arr_state);
    cJSON_AddStringToObject(root, "raw_is_raw", raw_state);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}