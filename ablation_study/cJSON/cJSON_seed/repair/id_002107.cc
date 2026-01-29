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
//<ID> 2107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON *pi = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(config, "pi", pi);
    double updated = cJSON_SetNumberHelper(pi, 6.28);
    cJSON_AddStringToObject(meta, "name", "cJSON_test");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON *pi_item = cJSON_GetObjectItem(detached, "pi");
    double got = cJSON_GetNumberValue(pi_item);
    char *printed = cJSON_PrintUnformatted(detached);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + (int)got);
    buffer[2] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}