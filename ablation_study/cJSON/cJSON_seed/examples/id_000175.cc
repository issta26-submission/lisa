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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    const char *pets_arr[] = { "cat", "dog", "bird" };
    cJSON *pets = cJSON_CreateStringArray(pets_arr, 3);
    cJSON_AddItemToObject(root, "pets", pets);

    // step 2: Configure
    cJSON *new_greeting = cJSON_CreateString("hi");
    cJSON_ReplaceItemInObject(root, "greeting", new_greeting);

    // step 3: Operate & Validate
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    char *greet_text = cJSON_GetStringValue(got_greeting);
    cJSON *greet_copy = cJSON_CreateString(greet_text);
    cJSON_AddItemToObject(root, "greeting_copy", greet_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}