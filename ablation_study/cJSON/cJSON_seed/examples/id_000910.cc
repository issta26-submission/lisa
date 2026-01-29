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
//<ID> 910
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
    cJSON *s2 = cJSON_CreateString("alpha");
    cJSON *t = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(root, s1);
    cJSON_AddItemToArray(root, t);
    cJSON_AddItemToArray(root, num);
    cJSON_AddItemToArray(root, s2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(root, 0);
    cJSON *last = cJSON_GetArrayItem(root, 3);
    cJSON_bool cmp = cJSON_Compare(first, last, 1);
    double newval = (double)cmp * 3.14159 + (double)(1 - cmp) * 2.71828;
    double set_res = cJSON_SetNumberHelper(num, newval);
    (void)set_res;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}