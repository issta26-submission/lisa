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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *child = NULL;
    cJSON *false_item = NULL;
    cJSON *got_child = NULL;
    cJSON *got_name = NULL;
    char *printed = NULL;
    char *name_value = NULL;
    cJSON_bool add_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "array", arr);
    false_item = cJSON_CreateFalse();
    add_res = cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate / Validate
    got_child = cJSON_GetObjectItem(root, "child");
    got_name = cJSON_GetObjectItem(got_child, "name");
    name_value = cJSON_GetStringValue(got_name);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}