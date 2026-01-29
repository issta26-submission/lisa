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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello-world");
    cJSON *raw_payload = cJSON_CreateRaw("{\"sensor\":42}");

    // step 2: Configure
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "payload", raw_payload);
    cJSON_bool greeting_is_string = cJSON_IsString(greeting);
    cJSON *is_string_item = cJSON_CreateBool(greeting_is_string);
    cJSON_AddItemToObject(root, "greeting_is_string", is_string_item);
    cJSON *raw_dup = cJSON_Duplicate(raw_payload, 0);
    cJSON_AddItemToObject(root, "payload_copy", raw_dup);

    // step 3: Operate & Validate
    cJSON *got = cJSON_GetObjectItem(root, "greeting");
    char *got_text = cJSON_GetStringValue(got);
    (void)got_text;
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_bool dup_equal = cJSON_Compare(root, root_dup, 1);
    cJSON_AddItemToObject(root_dup, "dup_equal", cJSON_CreateBool(dup_equal));

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}