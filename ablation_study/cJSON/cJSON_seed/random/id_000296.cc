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
//<ID> 296
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
    cJSON *label_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON *got_label = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw = cJSON_CreateRaw("unparsed-raw-fragment");
    label_item = cJSON_CreateString("label-value");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "my_array", arr);
    add_ok = cJSON_AddItemToObject(root, "my_raw", raw);
    cJSON_AddStringToObject(root, "my_label", "label-value");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42));
    cJSON_AddItemToArray(arr, cJSON_CreateString("element"));

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "my_array");
    got_raw = cJSON_GetObjectItem(root, "my_raw");
    got_label = cJSON_GetObjectItem(root, "my_label");
    is_arr = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    cJSON_AddItemToObject(root, "array_is_array", cJSON_CreateBool(is_arr));
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(is_raw));
    cJSON_AddItemToObject(root, "label_echo", cJSON_Duplicate(got_label, (cJSON_bool)1));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}