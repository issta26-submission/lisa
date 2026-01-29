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
//<ID> 2103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *pi_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON *ver_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "version_num", ver_item);

    // step 2: Configure
    cJSON_SetNumberHelper(pi_item, 3.14159);
    cJSON_SetNumberHelper(ver_item, 1.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const size_t buflen = 128;
    char *buffer = (char *)cJSON_malloc(buflen);
    memset(buffer, 0, buflen);
    buffer[0] = printed[0];
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON *detached_version = cJSON_GetObjectItem(detached_meta, "version_num");
    double ver_val = cJSON_GetNumberValue(detached_version);
    cJSON_SetNumberHelper(detached_version, ver_val + 1.0);
    buffer[1] = (char)('0' + (int)detached_version->valueint);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}