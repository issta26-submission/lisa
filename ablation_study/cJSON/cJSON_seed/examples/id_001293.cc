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
//<ID> 1293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemReferenceToObject(root, "items_ref", items_dup);
    cJSON *first_dup = cJSON_Duplicate(str_a, 1);
    cJSON_AddItemReferenceToObject(root, "first_ref", first_dup);
    cJSON_bool is_flag_true = cJSON_IsTrue(flag);
    (void)is_flag_true;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}