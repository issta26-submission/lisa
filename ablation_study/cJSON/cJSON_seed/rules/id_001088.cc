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
//<ID> 1088
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
    cJSON *false_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *first_elem = NULL;
    char *printed = NULL;
    char *retrieved_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "data");
    first_elem = cJSON_GetArrayItem(got_arr, 0);
    retrieved_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}