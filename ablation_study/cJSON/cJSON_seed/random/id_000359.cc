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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    const char *raw_json = (const char *)"{\"x\":[1,2,3]}";
    cJSON_bool arr_is_array = (cJSON_bool)0;
    cJSON_bool raw_is_array = (cJSON_bool)0;
    int combined_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "arr", arr);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_json);

    // step 4: Operate
    arr_is_array = cJSON_IsArray(arr);
    raw_is_array = cJSON_IsArray(raw_item);

    // step 5: Validate
    combined_flag = ((int)arr_is_array) + ((int)raw_is_array);
    (void)combined_flag;
    (void)arr_is_array;
    (void)raw_is_array;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}