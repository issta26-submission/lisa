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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *nothing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", nothing);

    // step 3: Operate & Validate
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    cJSON_AddBoolToObject(root, "child_is_object", child_is_obj);
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    cJSON_AddBoolToObject(root, "has_greeting", has_greeting);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}