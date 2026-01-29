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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2,3]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_InsertItemInArray(arr, 1, false_item);
    cJSON_bool false_is = cJSON_IsFalse(false_item);
    cJSON *flag_num = cJSON_CreateNumber((double)false_is);
    cJSON_AddItemToObject(root, "flag_numeric", flag_num);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}