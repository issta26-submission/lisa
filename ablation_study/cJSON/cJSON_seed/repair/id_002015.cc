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
//<ID> 2015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON *ref = cJSON_CreateStringReference("gamma");
    cJSON_AddItemToArray(array, ref);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_ReplaceItemViaPointer(array, s1, replacement);
    cJSON *extra = cJSON_CreateString("epsilon");
    cJSON_AddItemToArray(array, extra);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char c0 = printed[0];
    char c1 = printed[1];
    printed[0] = c0;
    printed[1] = c1;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}