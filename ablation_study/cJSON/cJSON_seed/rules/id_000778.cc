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
//<ID> 778
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *key_false = "active";
    const char *key_null = "data";
    cJSON *root = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    char *printed = NULL;
    const cJSON *retrieved_false = NULL;
    const cJSON *retrieved_null = NULL;
    cJSON_bool is_false = 0;
    cJSON_bool is_null = 0;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    false_item = cJSON_AddFalseToObject(root, key_false);
    null_item = cJSON_AddNullToObject(root, key_null);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    if (printed_len && printed_len < 256) {
        memcpy(buffer, printed, printed_len + 1);
    } else if (printed_len) {
        /* copy only up to buffer size minus one to keep null termination */
        memcpy(buffer, printed, 255);
        buffer[255] = '\0';
    }
    retrieved_false = cJSON_GetObjectItem(root, key_false);
    retrieved_null = cJSON_GetObjectItem(root, key_null);
    is_false = cJSON_IsFalse(retrieved_false);
    is_null = cJSON_IsNull(retrieved_null);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}