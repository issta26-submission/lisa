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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("Hello, cJSON!");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "stage", "configured");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(got_greeting);
    size_t greet_len = greet_val ? strlen(greet_val) : 0;
    cJSON *got_optional = cJSON_GetObjectItem(root, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    cJSON_AddNumberToObject(root, "len_plus_null", (double)greet_len + (double)optional_is_null);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}