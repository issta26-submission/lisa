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
//<ID> 1688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON *retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON *retrieved_active = cJSON_GetObjectItem(root, "active");

    // step 3: Operate and Validate
    cJSON_bool is_invalid = cJSON_IsInvalid(retrieved_greeting);
    cJSON_bool is_true = cJSON_IsTrue(retrieved_active);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_true);
    buffer[2] = (char)('0' + (int)is_invalid);
    const char *greet_val = cJSON_GetStringValue(retrieved_greeting);
    buffer[3] = greet_val ? greet_val[0] : 'Y';
    (void)buffer;
    (void)greet_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}