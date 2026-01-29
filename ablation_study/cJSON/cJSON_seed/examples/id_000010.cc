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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *str_first = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, str_first);
    cJSON *num_one = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, num_one);
    cJSON *str_third = cJSON_CreateString("third");
    cJSON_AddItemToArray(items, str_third);
    cJSON *ver = cJSON_AddNumberToObject(root, "version", 3.14159);
    cJSON *lbl = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", lbl);
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe_null");

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, cJSON_CreateNumber(2.0));
    cJSON *retrieved_null = cJSON_GetObjectItem(root, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(retrieved_null);
    cJSON *duplicate_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate_root, 1);

    // step 4: Cleanup
    cJSON_Delete(duplicate_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)is_null;
    (void)equal;
    (void)ver;
    return 66;
}