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
//<ID> 1455
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
    cJSON_bool is_true = 0;
    cJSON_bool print_ok = 0;
    double num_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    str_ref = cJSON_CreateStringReference("immutable_label");
    cJSON_AddItemToObject(root, "label", str_ref);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    num_val = cJSON_GetNumberValue(num_item);
    memset(prebuf, 0, sizeof(prebuf));
    print_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), is_true);
    (void)num_val;
    (void)print_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}