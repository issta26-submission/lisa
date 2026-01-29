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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"greeting\":\"hello\",\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_str = cJSON_CreateString("hello");
    cJSON *created_bool = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "greeting", created_str);
    cJSON_AddItemToObject(root, "flag", created_bool);

    // step 3: Operate & Validate
    cJSON *parsed_greeting = cJSON_GetObjectItem(parsed, "greeting");
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool greetings_match = cJSON_Compare(parsed_greeting, created_str, 1);
    cJSON_bool flags_match = cJSON_Compare(parsed_flag, created_bool, 1);
    cJSON_AddBoolToObject(root, "greetings_match", greetings_match);
    cJSON_AddBoolToObject(root, "flags_match", flags_match);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66;
    // API sequence test completed successfully
}