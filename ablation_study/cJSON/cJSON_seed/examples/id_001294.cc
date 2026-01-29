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
//<ID> 1294
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
    cJSON *label = cJSON_CreateString("label-value");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToObject(root, "label_ref", label);
    cJSON *add_ref_num = cJSON_CreateNumber((double)add_ref_ok);
    cJSON_AddItemToObject(root, "label_ref_added", add_ref_num);
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_dup", items_dup);

    // step 3: Operate & Validate
    cJSON_bool flag_is_true = cJSON_IsTrue(true_item);
    cJSON *flag_number = cJSON_CreateNumber((double)flag_is_true);
    cJSON_AddItemToObject(root, "flag_as_number", flag_number);
    int count = cJSON_GetArraySize(items);
    (void)count;
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}