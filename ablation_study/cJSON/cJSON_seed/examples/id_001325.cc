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
//<ID> 1325
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
    cJSON *truth = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON_AddItemToObjectCS(root, "is_valid", truth);

    // step 3: Operate & Validate
    const char *greet_val = cJSON_GetStringValue(greeting);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    size_t len = strlen(printed);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, printed, len + 1);
    (void)greet_val;
    cJSON_free(printed);
    cJSON_free(copy);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}