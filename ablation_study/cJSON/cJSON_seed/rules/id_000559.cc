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
//<ID> 559
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
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    const char *static_value = "hello_ref";
    cJSON *str_ref = cJSON_CreateStringReference(static_value);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON *num_added = cJSON_AddNumberToObject(root, "count", 2026.0);
    (void)num_added;
    cJSON_AddItemToObject(root, "greeting", str_ref);

    // step 3: Operate / Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "greeting");
    char *retrieved_str = cJSON_GetStringValue(retrieved);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 240 ? printed_len : 240;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    buffer[240] = retrieved_str ? retrieved_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}