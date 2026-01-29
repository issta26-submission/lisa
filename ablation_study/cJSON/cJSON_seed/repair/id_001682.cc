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
//<ID> 1682
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
    cJSON *active = cJSON_CreateTrue();
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "configured");
    cJSON_AddNumberToObject(root, "id", 7.0);

    // step 3: Operate and Validate
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(got_greeting);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(got_active);
    cJSON_bool greeting_invalid = cJSON_IsInvalid(got_greeting);
    char *printed = cJSON_Print(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = greet_val ? greet_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)active_is_true);
    buffer[3] = (char)('0' + (int)greeting_invalid);
    (void)buffer;
    (void)got_greeting;
    (void)got_active;
    (void)greet_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}