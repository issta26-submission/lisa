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
//<ID> 1452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *name_ref = NULL;
    cJSON *num_item = NULL;
    char prebuf[256];
    const char *name = "example_name";
    cJSON_bool is_true = 0;
    cJSON_bool print_ok = 0;
    int combined_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    name_ref = cJSON_CreateStringReference(name);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "name", name_ref);
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    memset(prebuf, 0, sizeof(prebuf));
    print_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    combined_check = (int)is_true + (int)print_ok;
    (void)combined_check;
    (void)num_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}