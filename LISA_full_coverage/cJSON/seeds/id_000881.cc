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
//<ID> 881
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", child);
    cJSON *flag = cJSON_AddTrueToObject(child, "enabled");

    // step 2: Configure
    cJSON_AddItemToObject(child, "name", cJSON_CreateString("fuzz_target"));
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(42.0));
    char *buf = (char*)cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buf, 512, 0);

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(child, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(retrieved);
    cJSON *dup_flag = cJSON_Duplicate(flag, 1);
    cJSON_AddItemToObject(root, "enabled_copy", dup_flag);
    cJSON_bool dup_is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled_copy"));

    // step 4: Validate & Cleanup
    (void)pre_ok;
    (void)is_true;
    (void)dup_is_true;
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}