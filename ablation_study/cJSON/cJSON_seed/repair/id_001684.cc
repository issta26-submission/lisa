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
//<ID> 1684
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "ok", flag);

    // step 2: Configure
    cJSON_AddStringToObject(root, "role", "tester");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "ok");
    cJSON_bool flag_is_true = cJSON_IsTrue(retrieved_flag);
    cJSON *retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool greeting_invalid = cJSON_IsInvalid(retrieved_greeting);
    const char *greet_text = cJSON_GetStringValue(retrieved_greeting);
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)flag_is_true);
    buffer[2] = (char)('0' + (int)greeting_invalid);
    buffer[3] = greet_text ? greet_text[0] : 'Y';
    (void)buffer;
    (void)meta;
    (void)retrieved_flag;
    (void)retrieved_greeting;
    (void)greet_text;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}