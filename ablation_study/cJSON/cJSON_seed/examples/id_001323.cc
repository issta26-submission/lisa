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
//<ID> 1323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON *pi = cJSON_CreateNumber(3.14159);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "is_enabled", flag);
    cJSON_AddItemToObjectCS(root, "message", greeting);
    cJSON_AddItemToObjectCS(root, "meta", sub);
    cJSON_AddItemToObjectCS(sub, "pi", pi);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t len = strlen(printed);
    char *backup = (char *)cJSON_malloc(len + 1);
    memset(backup, 0, len + 1);
    memcpy(backup, printed, len + 1);
    cJSON_free(printed);
    char *msg = cJSON_GetStringValue(greeting);
    cJSON *msg_copy = cJSON_CreateString(msg);
    cJSON_AddItemToObjectCS(root, "message_copy", msg_copy);
    char *final_print = cJSON_PrintBuffered(root, 64, 0);
    cJSON_free(final_print);
    cJSON_free(backup);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}