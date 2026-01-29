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
//<ID> 1160
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
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *vals = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(vals, n1);
    cJSON_AddItemToArray(vals, n2);
    cJSON_AddItemToObject(root, "values", vals);
    cJSON_AddStringToObject(root, "name", "original");
    cJSON_AddNullToObject(cfg, "optional");

    // step 2: Configure
    cJSON_AddStringToObject(cfg, "mode", "active");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *cfg_ref = cJSON_GetObjectItem(root, "config");
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg_ref);
    cJSON *opt_ref = cJSON_GetObjectItem(cfg_ref, "optional");
    cJSON_bool opt_is_null = cJSON_IsNull(opt_ref);
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    cJSON_AddItemToObject(cfg_ref, "moved_name", detached_name);
    cJSON *vals_ref = cJSON_GetObjectItem(root, "values");
    cJSON *first_val = cJSON_GetArrayItem(vals_ref, 0);
    double set_ret = cJSON_SetNumberHelper(first_val, 99.5);
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(vals_ref, 1));
    cJSON *computed = cJSON_CreateNumber(set_ret + second_val);
    cJSON_AddItemToObject(root, "computed", computed);
    (void)cfg_is_obj;
    (void)opt_is_null;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}