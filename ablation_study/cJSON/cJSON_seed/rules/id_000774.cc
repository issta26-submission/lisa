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
//<ID> 774
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *msg_text = "hello";
    size_t buffer_size = 256;
    char *buffer = (char *)cJSON_malloc(buffer_size);
    memset(buffer, 0, buffer_size);
    char *printed = NULL;
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *str_item = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    const char *retrieved = NULL;
    size_t printed_len = 0;
    size_t copy_len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddStringToObject(nested, "message", msg_text);
    false_item = cJSON_AddFalseToObject(root, "enabled");
    null_item = cJSON_AddNullToObject(root, "payload");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    copy_len = (printed_len < (buffer_size - 1)) ? printed_len : (buffer_size - 1);
    if (printed && copy_len) {
        memcpy(buffer, printed, copy_len);
    }
    buffer[copy_len] = '\0';
    str_item = cJSON_GetObjectItem(nested, "message");
    retrieved = cJSON_GetStringValue(str_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}