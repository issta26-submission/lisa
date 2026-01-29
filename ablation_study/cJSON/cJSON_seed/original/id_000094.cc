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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", num);
    int ids_vals[3] = {10, 20, 30};
    cJSON *ids = cJSON_CreateIntArray(ids_vals, 3);
    cJSON_AddItemToObject(root, "ids", ids);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buf, (int)sizeof(buf), 1);

    // step 3: Operate & Validate
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    int summary = (int)pre_ok + (int)flag_is_true + (int)meta_is_obj + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}