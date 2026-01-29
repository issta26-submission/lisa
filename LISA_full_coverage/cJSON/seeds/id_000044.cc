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
//<ID> 44
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n2);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(items, 1, n1);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON_AddItemToObject(root, "flag_false", flag_false);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *mid = cJSON_GetArrayItem(items, 1);
    double mid_val = cJSON_GetNumberValue(mid);
    cJSON *got_true = cJSON_GetObjectItemCaseSensitive(root, "flag_true");
    cJSON *got_false = cJSON_GetObjectItemCaseSensitive(root, "flag_false");
    int true_flag = cJSON_IsTrue(got_true);
    int false_flag = cJSON_IsFalse(got_false);
    int summary = size + (int)mid_val + true_flag + false_flag + (int)cJSON_GetNumberValue(count_item);
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}