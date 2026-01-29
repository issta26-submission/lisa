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
//<ID> 552
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
    cJSON *num_item = NULL;
    cJSON *name_ref = NULL;
    char *name_value = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    size_t copy_len = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "active", true_item);
    num_item = cJSON_AddNumberToObject(root, "score", 88.5);
    name_ref = cJSON_CreateStringReference("tester");
    cJSON_AddItemToObject(root, "name", name_ref);

    // step 3: Operate / Validate
    name_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    copy_len = printed_len > 240 ? 240 : printed_len;
    buffer[0] = name_value[0];
    memcpy(buffer + 1, printed, copy_len);
    buffer[1 + copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}