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
//<ID> 357
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
    cJSON *raw_item = (cJSON *)0;
    cJSON_bool child_is_array = (cJSON_bool)0;
    cJSON_bool root_is_array = (cJSON_bool)0;
    char *raw_valuestring = (char *)0;
    const char *raw_json = "{\"numbers\": [1, 2, 3]}";

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    raw_item = cJSON_AddRawToObject(child, "rawdata", raw_json);

    // step 4: Operate
    child_is_array = cJSON_IsArray(child);
    root_is_array = cJSON_IsArray(root);
    raw_valuestring = raw_item ? raw_item->valuestring : (char *)0;

    // step 5: Validate
    (void)child_is_array;
    (void)root_is_array;
    (void)raw_valuestring;

    // step 6: Cleanup
    cJSON_Delete(root);
    root = (cJSON *)0;
    child = (cJSON *)0;
    raw_item = (cJSON *)0;

    // API sequence test completed successfully
    return 66;
}