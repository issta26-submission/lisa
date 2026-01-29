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
//<ID> 217
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
    cJSON *true_in_array = (cJSON *)0;
    cJSON *added_true_obj = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    cJSON *is_array_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool is_array_flag = (cJSON_bool)0;
    const char *version = (const char *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    added_true_obj = cJSON_AddTrueToObject(root, "present");
    true_in_array = cJSON_CreateTrue();
    add_ok1 = cJSON_AddItemToArray(arr, true_in_array);

    // step 4: Operate
    version = cJSON_Version();
    version_item = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "version", version_item);
    is_array_flag = cJSON_IsArray(arr);
    is_array_item = cJSON_CreateBool(is_array_flag);
    cJSON_AddItemToObject(root, "is_array", is_array_item);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}