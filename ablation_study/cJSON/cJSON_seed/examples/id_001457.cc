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
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON_AddItemToArray(root, s1);
    cJSON_AddItemToArray(root, btrue);

    // step 2: Configure
    cJSON *s_temp = cJSON_CreateString("alpha");
    cJSON_bool strings_equal = cJSON_Compare(s1, s_temp, 1);
    cJSON_Delete(s_temp);
    cJSON *b_same = cJSON_CreateBool(strings_equal);
    cJSON_AddItemToArray(root, b_same);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool roots_equal = cJSON_Compare(root, dup, 1);
    cJSON *b_roots_equal = cJSON_CreateBool(roots_equal);
    cJSON_AddItemToArray(dup, b_roots_equal);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}