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
//<ID> 2465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *true_item = cJSON_AddTrueToObject(child, "enabled");
    cJSON_AddItemToObject(root, "config", child);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';

    // step 3: Operate
    cJSON_bool was_false = cJSON_IsFalse(true_item);
    buffer[1] = (char)(int)was_false;
    char *reprinted = cJSON_Print(root);
    buffer[2] = reprinted ? reprinted[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(reprinted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}