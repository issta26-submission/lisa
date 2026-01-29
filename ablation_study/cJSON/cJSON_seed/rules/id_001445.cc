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
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *num1 = NULL;
    cJSON *nul = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool pre_ok = 0;
    cJSON_bool replace_ok = 0;
    cJSON_bool is_null_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(42.0);
    nul = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "num", num1);
    cJSON_AddItemToObjectCS(child, "nul", nul);
    cJSON_AddItemToObjectCS(root, "child", child);
    raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"injected\":true}");

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(100.0);
    replace_ok = cJSON_ReplaceItemViaPointer(child, num1, replacement);
    is_null_res = cJSON_IsNull(nul);
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    (void)replace_ok;
    (void)is_null_res;
    (void)raw_item;
    (void)pre_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}