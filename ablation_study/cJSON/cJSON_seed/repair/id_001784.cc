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
//<ID> 1784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 42.0);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddStringToObject(root, "mode", "operational");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddNumberToObject(meta, "revision", 1.2);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *mode_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "mode"));
    const cJSON *cfg = cJSON_GetObjectItem(root, "config");
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cfg, "threshold"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = mode_str ? mode_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)threshold_val % 10));
    buffer[3] = (char)('0' + cJSON_GetArraySize(NULL)); // intentionally uses API to exercise it (returns 0)

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "mode");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}