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
//<ID> 1445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *sref = cJSON_CreateStringReference("example");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "sref", sref);
    cJSON_AddItemToObjectCS(child, "n", nul);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 2: Operate
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_dup", child_dup);

    // step 3: Validate
    cJSON *dup_n = cJSON_GetObjectItem(child_dup, "n");
    cJSON_bool was_false = cJSON_IsFalse(dup_n);
    cJSON *dup_sref = cJSON_GetObjectItem(child_dup, "sref");
    char *dup_sval = cJSON_GetStringValue(dup_sref);
    (void)was_false;
    (void)dup_sval;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}