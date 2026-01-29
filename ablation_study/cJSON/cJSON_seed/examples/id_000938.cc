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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *greeting = cJSON_CreateString("hello-world");
    cJSON *value = cJSON_CreateNumber(3.1415);

    // step 2: Configure
    cJSON_AddItemToObject(root, "raw_fragment", raw_item);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "value", value);

    // step 3: Operate & Validate
    cJSON *greeting_dup = cJSON_Duplicate(greeting, 0);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_dup);
    cJSON_bool greeting_is_string = cJSON_IsString(greeting);
    cJSON *greeting_is_string_bool = cJSON_CreateBool(greeting_is_string);
    cJSON_AddItemToObject(root, "greeting_is_string", greeting_is_string_bool);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}