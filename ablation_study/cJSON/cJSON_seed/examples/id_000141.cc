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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "maybe_null", maybe_null);

    // step 3: Operate & Validate
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    (void)has_greeting;
    cJSON *retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    const char *greeting_value = cJSON_GetStringValue(retrieved_greeting);
    cJSON *greeting_dup = cJSON_CreateString(greeting_value);
    cJSON_AddItemToObject(root, "greeting_dup", greeting_dup);
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(retrieved_meta);
    (void)meta_is_obj;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}