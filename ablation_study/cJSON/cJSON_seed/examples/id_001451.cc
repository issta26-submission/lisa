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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *true_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_bool added_str = cJSON_AddItemToArray(root, str_item);
    cJSON_bool added_true = cJSON_AddItemToArray(root, true_item);

    // step 3: Operate & Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *cmp_flag = cJSON_CreateBool(equal);
    cJSON_bool added_cmp = cJSON_AddItemToArray(root, cmp_flag);

    // step 4: Cleanup
    if (unformatted) { cJSON_free(unformatted); }
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)added_str;
    (void)added_true;
    (void)added_cmp;
    return 66;
}