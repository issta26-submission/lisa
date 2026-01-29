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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement_num = NULL;
    cJSON *compare_item = NULL;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool replaced = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "num", num_item);
    str_item = cJSON_CreateString("payload");
    cJSON_AddItemToObject(root, "text", str_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "opt", null_item);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(null_item);
    is_false = cJSON_IsFalse(false_item);
    replacement_num = cJSON_CreateNumber(20.0);
    replaced = cJSON_ReplaceItemInObject(root, "num", replacement_num);
    compare_item = cJSON_Duplicate(replacement_num, 1);
    equal = cJSON_Compare(replacement_num, compare_item, 1);

    // step 4: Cleanup
    cJSON_Delete(compare_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)is_null;
    (void)is_false;
    (void)replaced;
    (void)equal;
    return 66;
}