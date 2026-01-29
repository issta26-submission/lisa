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
//<ID> 887
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_bool add_cfg_ok = cJSON_AddItemToObject(root, "config", cfg);
    cJSON *flag = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(cfg, "count", count);
    char *tmp_buf = (char*)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    tmp_buf[0] = '{';
    tmp_buf[1] = '}';
    tmp_buf[2] = '\0';

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON *cfg_copy = cJSON_Duplicate(cfg, 1);
    cJSON_AddItemToObject(root, "config_copy", cfg_copy);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *retrieved_flag = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool check_true = cJSON_IsTrue(retrieved_flag);
    (void)add_cfg_ok;
    (void)flag_true;
    (void)check_true;
    (void)tmp_buf;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}