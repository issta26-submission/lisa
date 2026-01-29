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
//<ID> 290
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
    cJSON *greeting_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON *got_greeting = (cJSON *)0;
    cJSON *flag_array_num = (cJSON *)0;
    cJSON *flag_raw_num = (cJSON *)0;
    char *greet_val = (char *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("unparsed:data_blob");
    greeting_item = cJSON_AddStringToObject(root, "greeting", "hello world");
    cJSON_AddItemToObject(root, "data_array", arr);
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 3: Operate
    got_arr = cJSON_GetObjectItem(root, "data_array");
    got_raw = cJSON_GetObjectItem(root, "raw_blob");
    got_greeting = cJSON_GetObjectItem(root, "greeting");
    greet_val = cJSON_GetStringValue(got_greeting);
    cJSON_AddStringToObject(root, "greeting_copy", greet_val);

    // step 4: Validate
    is_array = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);
    flag_array_num = cJSON_CreateNumber((double)is_array);
    flag_raw_num = cJSON_CreateNumber((double)is_raw);
    cJSON_AddItemToObject(root, "is_array", flag_array_num);
    cJSON_AddItemToObject(root, "is_raw", flag_raw_num);

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: API sequence test completed successfully
    return 66;
}