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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *str = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "greet", str);

    // step 2: Configure
    cJSON *found_num = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double initial_pi = cJSON_GetNumberValue(found_num);
    cJSON *found_str = cJSON_GetObjectItemCaseSensitive(root, "greet");
    cJSON_bool greet_is_string = cJSON_IsString(found_str);

    // step 3: Operate and Validate
    cJSON *new_num = cJSON_CreateNumber(6.28318);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "pi", new_num);
    cJSON *updated_num = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double updated_pi = cJSON_GetNumberValue(updated_num);
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_Minify(serialized);
    int summary = (int)initial_pi + (int)updated_pi + (int)greet_is_string + (int)replaced;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}