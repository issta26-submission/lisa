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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_ref = NULL;
    cJSON *true_item = NULL;
    char prebuf[256];
    cJSON_bool is_true = 0;
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_ref = cJSON_CreateStringReference("constant_string");
    cJSON_AddItemToObject(root, "ref", str_ref);
    cJSON_AddNumberToObject(root, "answer", 42.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), is_true);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}