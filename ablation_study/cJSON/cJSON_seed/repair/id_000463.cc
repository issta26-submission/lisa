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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[10,20]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool initial_is_false = cJSON_IsFalse(false_item);
    (void)initial_is_false;

    // step 2: Configure
    cJSON_bool inserted = cJSON_InsertItemInArray(numbers, 1, false_item);
    (void)inserted;

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON *retrieved = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool retrieved_is_false = cJSON_IsFalse(retrieved);
    (void)retrieved_is_false;

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}