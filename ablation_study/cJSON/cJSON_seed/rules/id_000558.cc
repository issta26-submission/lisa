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
//<ID> 558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_ref = cJSON_CreateStringReference("hello_world");
    cJSON *num_item = NULL;
    char *printed = NULL;
    char *retrieved_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "enabled", true_item);
    num_item = cJSON_AddNumberToObject(root, "version", 2.718);
    cJSON_AddItemToObject(root, "greeting", str_ref);

    // step 3: Operate / Validate
    retrieved_str = cJSON_GetStringValue(str_ref);
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 120 ? 120 : printed_len;
    buffer[0] = retrieved_str[0];
    memcpy(buffer + 1, printed, copy_len);
    buffer[1 + copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}