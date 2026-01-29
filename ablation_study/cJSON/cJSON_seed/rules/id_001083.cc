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
//<ID> 1083
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
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *first_elem = NULL;
    const char *extracted_str = NULL;
    char *printed = NULL;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("hello_world");
    add_res1 = cJSON_AddItemToArray(arr, str_item);
    bool_item = cJSON_CreateFalse();
    add_res2 = cJSON_AddItemToArray(arr, bool_item);

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "items");
    first_elem = cJSON_GetArrayItem(got_arr, 0);
    extracted_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}