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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s_item = NULL;
    cJSON *s_ref = NULL;
    cJSON *meta = NULL;
    cJSON *greeting_item = NULL;
    const char *greeting_value = NULL;
    char *printed = NULL;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s_item = cJSON_CreateString("alpha");
    meta = cJSON_CreateObject();

    // If any allocation failed, clean up what we have and return (preserve behavior)
    if (!root || !arr || !s_item || !meta) {
        if (printed) cJSON_free(printed);
        cJSON_Delete(root);
        cJSON_Delete(arr);
        cJSON_Delete(s_item);
        cJSON_Delete(meta);
        return 66;
    }

    // step 3: Configure
    cJSON_AddItemToArray(arr, s_item);

    // Use a duplicate of s_item rather than CreateObjectReference to avoid double-free
    s_ref = cJSON_Duplicate(s_item, 1);
    if (s_ref) {
        cJSON_AddItemToArray(arr, s_ref);
    } else {
        // If duplication failed, continue with single item in array
    }

    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(meta, "greeting", "hello world");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    greeting_item = cJSON_GetObjectItem(meta, "greeting");
    greeting_value = cJSON_GetStringValue(greeting_item);

    // step 5: Validate / print
    printed = cJSON_PrintUnformatted(root);
    (void)greeting_value;
    (void)printed;

    // step 6: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}