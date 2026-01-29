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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("example");
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "answer", n1);
    cJSON_AddItemToObject(root, "label", s1);
    cJSON_AddItemToObject(root, "ok", btrue);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool ok_flag = cJSON_IsTrue(btrue);
    cJSON *derived_num = cJSON_CreateNumber((double)ok_flag + 1.5);
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    cJSON *label_copy = cJSON_CreateString(label_str);
    cJSON_AddItemToObject(dup, "derived", derived_num);
    cJSON_AddItemToObject(dup, "label_copy", label_copy);

    // step 3: Operate & Validate
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    cJSON_AddBoolToObject(root, "equal", are_equal);
    cJSON *eq_item = cJSON_GetObjectItem(root, "equal");
    cJSON_bool eq_true = cJSON_IsTrue(eq_item);
    cJSON *eq_num = cJSON_CreateNumber((double)eq_true);
    cJSON_AddItemToObject(root, "equal_num", eq_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}