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
//<ID> 848
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *placeholder = cJSON_CreateString("to_be_replaced");
    cJSON_AddItemToObject(root, "replace_me", placeholder);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 314.159);

    // step 2: Configure
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.234));
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate
    char json_buf[] = " { \"alpha\": 10, \"beta\": [ 1, 2, 3 ] } ";
    cJSON_Minify(json_buf);
    size_t buf_len = sizeof(json_buf) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json_buf, buf_len);
    cJSON *parsed_newnum = cJSON_CreateNumber(99.9);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "alpha", parsed_newnum);

    // step 4: Validate & Cleanup
    double val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(parsed, "alpha"));
    (void)val;
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}