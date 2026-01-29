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
//<ID> 1459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_ref = NULL;
    cJSON *true_item = NULL;
    char prebuf[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "value", 42.0);
    str_ref = cJSON_CreateStringReference("status_ok");
    cJSON_AddItemToObject(root, "status", str_ref);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    memset(prebuf, 0, sizeof(prebuf));
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    flag_true = cJSON_IsTrue(true_item);
    (void)printed_ok;
    (void)flag_true;
    (void)num_item;
    (void)str_ref;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}