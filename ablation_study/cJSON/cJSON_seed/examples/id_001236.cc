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
//<ID> 1236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double numbers[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *plain_array = cJSON_CreateArray();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON_AddItemToObject(root, "list", plain_array);
    cJSON *elem_num = cJSON_CreateNumber(10.0);
    cJSON *elem_str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(plain_array, elem_num);
    cJSON_AddItemToArray(plain_array, elem_str);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 3: Operate & Validate
    cJSON *got_null = cJSON_GetObjectItem(root, "empty");
    cJSON_bool null_flag = cJSON_IsNull(got_null);
    cJSON_AddBoolToObject(root, "null_flag", null_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}