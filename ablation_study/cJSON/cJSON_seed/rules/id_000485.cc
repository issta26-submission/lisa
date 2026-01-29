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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *added_message = NULL;
    cJSON *parsed = NULL;
    cJSON *active_item = NULL;
    cJSON *message_item = NULL;
    const char *message_str = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    added_message = cJSON_AddStringToObject(root, "message", "hello world");
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    active_item = cJSON_GetObjectItem(parsed, "active");
    message_item = cJSON_GetObjectItem(parsed, "message");
    message_str = cJSON_GetStringValue(message_item);
    buffer[0] = (char)('0' + (int)cJSON_IsTrue(active_item));
    buffer[1] = message_str[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}