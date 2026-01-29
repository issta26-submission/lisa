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
//<ID> 1435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddNumberToObject(settings, "threshold", 3.1415);

    // step 3: Operate & Validate
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    cJSON_bool ver_is_num = cJSON_IsNumber(ver);
    cJSON *ver_dup = cJSON_Duplicate((const cJSON *)ver, 0);
    cJSON_AddItemToObject(root, "version_copy", ver_dup);
    const int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_free((void *)buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)ver_is_num;
    (void)printed;
    return 66;
}