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
//<ID> 1457
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
    cJSON_bool print_ok = 0;
    cJSON_bool is_true = 0;
    char buffer[256];
    const char *static_str = "example string";
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    str_ref = cJSON_CreateStringReference(static_str);
    cJSON_AddItemToObject(root, "greeting", str_ref);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "alive", true_item);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(true_item);
    print_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)is_true;
    (void)print_ok;
    (void)num_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}