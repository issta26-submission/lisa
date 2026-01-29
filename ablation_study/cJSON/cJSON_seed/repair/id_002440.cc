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
//<ID> 2440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddNumberToObject(root, "e", 2.71828);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(root, "pi_adjusted", pi_val + 0.001);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[1] = printed2 && printed2[0] ? printed2[0] : '\0';
    buffer[2] = (char)(int)is_active;

    // step 4: Validate and Cleanup
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(got_meta, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    buffer[3] = (char)(int)version_val;
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}