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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"alpha\":null,\"beta\":false,\"name\":\"orig_name\",\"value\":42}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *synth = cJSON_CreateObject();
    cJSON *created_str = cJSON_CreateString("synthesized_entry");
    cJSON_AddItemToObject(synth, "created_name", created_str);
    cJSON *orig_name_item = cJSON_GetObjectItem(parsed, "name");
    char *orig_name_val = cJSON_GetStringValue(orig_name_item);
    cJSON *copied_name = cJSON_CreateString(orig_name_val);
    cJSON_AddItemToObject(synth, "copied_name", copied_name);
    cJSON_bool alpha_was_null = cJSON_IsNull(cJSON_GetObjectItem(parsed, "alpha"));
    cJSON_AddBoolToObject(synth, "alpha_was_null", alpha_was_null);
    cJSON_AddBoolToObject(synth, "beta_flag", cJSON_IsTrue(cJSON_GetObjectItem(parsed, "beta")));

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(synth, 1);
    cJSON_bool equal = cJSON_Compare(synth, dup, 1);
    cJSON_AddBoolToObject(synth, "dup_equal", equal);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(synth);

    // API sequence test completed successfully
    return 66;
}