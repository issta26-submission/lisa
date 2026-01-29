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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON_AddItemToObjectCS(child, "number", num);
    cJSON_AddItemToObjectCS(child, "text", str);
    cJSON_AddItemToObjectCS(root, "array", arr);
    cJSON_AddItemToObjectCS(root, "empty", null_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    // step 3: Operate & Validate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON *got_child = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *got_number = cJSON_GetObjectItemCaseSensitive(got_child, "number");
    cJSON *got_empty = cJSON_GetObjectItemCaseSensitive(root, "empty");
    cJSON_bool empty_is_null = cJSON_IsNull(got_empty);
    double number_val = cJSON_GetNumberValue(got_number);
    int summary = (printed ? (int)printed[0] : 0) + (int)empty_is_null + (int)number_val + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}