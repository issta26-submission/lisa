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
//<ID> 1442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child_a = NULL;
    cJSON *child_b = NULL;
    cJSON *replacement = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool pre_ok = 0;
    cJSON_bool null_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child_a = cJSON_CreateString("value_a");
    child_b = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "alpha", child_a);
    cJSON_AddItemToObjectCS(root, "beta", child_b);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(3.1415);
    cJSON_ReplaceItemViaPointer(root, child_a, replacement);
    null_check = cJSON_IsNull(child_b);
    raw_item = cJSON_AddRawToObject(root, "rawdata", "{\"r\":true}");
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    (void)printed;
    (void)pre_ok;
    (void)null_check;
    (void)raw_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}