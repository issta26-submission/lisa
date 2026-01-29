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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag", flag);

    // step 3: Operate and Validate
    cJSON *found_child = cJSON_GetObjectItem(root, "child");
    cJSON *found_greeting = cJSON_GetObjectItem(found_child, "greeting");
    const char *greeting_val = cJSON_GetStringValue(found_greeting);
    cJSON *found_flag = cJSON_GetObjectItem(found_child, "flag");
    int valid_flag = (int)cJSON_IsTrue(found_flag);
    double name_check = greeting_val ? 1.0 : 0.0;
    cJSON_AddNumberToObject(root, "validation", name_check + (double)valid_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}