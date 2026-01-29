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
//<ID> 1087
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
    cJSON *false_item = NULL;
    cJSON *got_item = NULL;
    cJSON *new_str = NULL;
    char *printed = NULL;
    char *str_val = NULL;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "name", "example_name");
    false_item = cJSON_CreateFalse();
    add_res1 = cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "name");
    str_val = cJSON_GetStringValue(got_item);
    new_str = cJSON_CreateString(str_val);
    add_res2 = cJSON_AddItemToArray(arr, new_str);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}