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
//<ID> 2387
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
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(config, "value", num_item);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddNumberToObject(root, "threshold", 42.0);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    cJSON *thr_item = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *val_item = cJSON_GetObjectItem(cfg, "value");
    double val_num = cJSON_GetNumberValue(val_item);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)thr_val;
    buffer[2] = (char)(int)val_num;
    buffer[3] = (char)(int)has_enabled;
    (void)enabled_item;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}