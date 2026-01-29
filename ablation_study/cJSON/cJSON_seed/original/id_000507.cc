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
//<ID> 507
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
    cJSON *bool_item = cJSON_AddBoolToObject(root, "ok", 1);

    // step 2: Configure
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    char *greet_val = cJSON_GetStringValue(got_greeting);
    cJSON_AddStringToObject(root, "echo", greet_val);
    cJSON *ok_item = cJSON_GetObjectItem(root, "ok");

    // step 3: Operate
    cJSON_bool ok_is_true = cJSON_IsTrue(ok_item);
    cJSON_bool greeting_is_raw = cJSON_IsRaw(got_greeting);
    double flags_sum = (double)ok_is_true + (double)greeting_is_raw;
    cJSON_AddNumberToObject(root, "flags_sum", flags_sum);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)flags_sum;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}