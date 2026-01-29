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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *child_num = cJSON_AddNumberToObject(nested, "child_val", 42.0);

    // step 3: Operate and Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    char *pi_str = cJSON_Print(got_pi);
    cJSON *got_nested = cJSON_GetObjectItem(root, "nested");
    char *nested_unformatted = cJSON_PrintUnformatted(got_nested);
    (void)pi_item;
    (void)child_num;
    (void)got_pi;
    (void)got_nested;

    // step 4: Cleanup
    cJSON_free(pi_str);
    cJSON_free(nested_unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}