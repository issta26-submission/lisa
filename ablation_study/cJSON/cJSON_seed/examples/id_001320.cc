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
//<ID> 1320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *s_item = cJSON_CreateString("hello world");
    cJSON *t_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "greeting", s_item);
    cJSON_AddItemToObjectCS(root, "enabled", t_item);

    // step 3: Operate & Validate
    char *greeting = cJSON_GetStringValue(s_item);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    size_t printed_len = strlen(printed);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, printed, printed_len + 1);
    cJSON_free(printed);
    cJSON_free(copy);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}