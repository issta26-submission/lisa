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
//<ID> 295
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
    cJSON *str_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    cJSON *dup_str = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_raw = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    double arr_flag = 0.0;
    double raw_flag = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw = cJSON_CreateRaw("raw_payload");

    // step 3: Configure
    // cJSON_AddStringToObject both creates and adds the item to 'root'
    str_item = cJSON_AddStringToObject(root, "greeting", "hello world");
    add_ok_arr = cJSON_AddItemToObject(root, "items", arr);
    add_ok_raw = cJSON_AddItemToObject(root, "payload", raw);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "items");
    got_raw = cJSON_GetObjectItem(root, "payload");
    got_str = cJSON_GetObjectItem(root, "greeting");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    arr_flag = (double)is_arr;
    raw_flag = (double)is_raw;
    cJSON_AddNumberToObject(root, "items_is_array", arr_flag);
    cJSON_AddNumberToObject(root, "payload_is_raw", raw_flag);

    // Avoid adding the same internal pointer twice (which would cause double free).
    // Instead duplicate the string item and add the duplicate to the object.
    if (got_str != (cJSON *)0) {
        dup_str = cJSON_Duplicate(got_str, (cJSON_bool)1); // recurse = true to copy string value
        if (dup_str != (cJSON *)0) {
            cJSON_AddItemToObject(root, "original_greeting", dup_str);
        } else {
            // If duplication failed for some reason, add a safe fallback string
            cJSON_AddStringToObject(root, "original_greeting", cJSON_GetStringValue(got_str));
        }
    } else {
        // If greeting wasn't present, add a null to indicate absence
        cJSON_AddNullToObject(root, "original_greeting");
    }

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}