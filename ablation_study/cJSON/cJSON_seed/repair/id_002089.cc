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
//<ID> 2089
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *t_item = cJSON_CreateTrue();
    cJSON *f_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, t_item);
    cJSON_AddItemToArray(flags, f_item);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "cJSON_fuzz");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(flags, 0);
    cJSON_bool was_true = cJSON_IsTrue(detached);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (int)was_true);
    buffer[1] = printed ? printed[0] : '\0';
    buffer[2] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}