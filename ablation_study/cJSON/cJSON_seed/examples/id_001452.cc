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
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("alpha");
    cJSON *bool_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(root, str1);
    cJSON_AddItemToArray(root, str2);
    cJSON_AddItemToArray(root, bool_item);

    // step 3: Operate & Validate
    cJSON_bool cmp = cJSON_Compare(str1, str2, 1);
    cJSON *cmp_num = cJSON_CreateNumber((double)cmp);
    cJSON_AddItemToArray(root, cmp_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}