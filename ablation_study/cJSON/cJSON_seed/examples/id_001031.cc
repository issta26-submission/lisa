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
//<ID> 1031
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("answer");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *n2 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", n2);
    cJSON *label = cJSON_CreateString("const_pi");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate & Validate
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON *equal_num = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal_flag", equal_num);
    cJSON_bool is_true = cJSON_IsTrue(flag);
    cJSON *istrue_num = cJSON_CreateNumber((double)is_true);
    cJSON_AddItemToObject(root, "is_true_flag", istrue_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_Minify(out);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}