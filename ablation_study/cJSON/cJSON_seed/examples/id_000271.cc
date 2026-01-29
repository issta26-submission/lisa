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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_bool r1 = cJSON_AddItemToArray(arr, num1);
    cJSON_bool r2 = cJSON_AddItemToArray(arr, num2);
    cJSON_bool r3 = cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON *added_count = cJSON_CreateNumber((double)(r1 + r2 + r3));
    cJSON_AddItemToObjectCS(root, "added_count", added_count);

    // step 3: Operate & Validate
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    cJSON *false_flag = cJSON_CreateBool(was_false);
    cJSON_AddItemToObjectCS(root, "false_was_false", false_flag);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *printed_flag = cJSON_CreateBool(printed_ok);
    cJSON_AddItemToObjectCS(root, "printed_ok", printed_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}