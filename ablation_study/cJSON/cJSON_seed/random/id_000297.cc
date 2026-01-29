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
//<ID> 297
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
    cJSON *got_title = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    char *retrieved_title = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", "Hello cJSON");
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"raw\":true}");

    // step 3: Configure
    cJSON_AddItemToObject(root, "data_array", arr);
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "data_array");
    got_raw = cJSON_GetObjectItem(root, "raw_blob");
    got_title = cJSON_GetObjectItem(root, "title");
    is_array = cJSON_IsArray(got_arr);
    is_raw = cJSON_IsRaw(got_raw);
    retrieved_title = cJSON_GetStringValue(got_title);
    cJSON_AddStringToObject(root, "copied_title", retrieved_title);

    // step 5: Validate
    cJSON_AddItemToObject(root, "array_is_array", cJSON_CreateBool(is_array));
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(is_raw));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}