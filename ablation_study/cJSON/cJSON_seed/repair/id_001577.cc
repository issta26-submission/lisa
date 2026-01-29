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
//<ID> 1577
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
    cJSON *threshold = cJSON_CreateNumber(3.1415);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg);
    cJSON *th_item = cJSON_GetObjectItem(cfg, "threshold");
    double th_val = cJSON_GetNumberValue(th_item);
    cJSON *double_th = cJSON_CreateNumber(th_val * 2.0);
    cJSON_AddItemToObject(root, "double_threshold", double_th);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)cfg_is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)equal);
    scratch[3] = (char)('0' + (int)((int)th_val % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}