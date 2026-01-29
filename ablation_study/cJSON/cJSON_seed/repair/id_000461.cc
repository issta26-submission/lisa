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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[true,true],\"meta\":{}}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *array = cJSON_GetObjectItem(root, "arr");
    cJSON_InsertItemInArray(array, 1, false_item);

    // step 3: Operate
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *flag_number = cJSON_CreateNumber((double)was_false);
    cJSON_AddItemToObject(dup, "false_flag", flag_number);

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(dup);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}