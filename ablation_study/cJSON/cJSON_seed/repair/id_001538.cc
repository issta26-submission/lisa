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
//<ID> 1538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "app", "cjson_fuzz");
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(cfg, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(cfg, "threshold", threshold);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(cfg, "count", count);

    // step 3: Operate and Validate
    double thr = cJSON_GetNumberValue(threshold);
    double dbl = thr * 2.0;
    cJSON *threshold_x2 = cJSON_CreateNumber(dbl);
    cJSON_AddItemToObject(cfg, "threshold_x2", threshold_x2);
    double read_back = cJSON_GetNumberValue(threshold_x2);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    int i_thr = (int)thr;
    int i_read = (int)read_back;
    scratch[0] = (char)('0' + (i_thr % 10));
    scratch[1] = (char)('0' + (i_read % 10));
    scratch[2] = 'A';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}