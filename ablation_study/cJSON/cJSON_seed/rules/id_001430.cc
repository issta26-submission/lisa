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
//<ID> 1430
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
    cJSON *detached = NULL;
    char *printed = NULL;
    char *extracted_string = NULL;
    cJSON_bool raw_flag_detached = 0;
    cJSON_bool raw_flag_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("detached_string");
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(arr, str_item);
    extracted_string = cJSON_GetStringValue(detached);
    raw_flag_detached = cJSON_IsRaw(detached);
    raw_flag_false = cJSON_IsRaw(false_item);
    printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}