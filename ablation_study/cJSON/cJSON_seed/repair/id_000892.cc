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
//<ID> 892
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *num_item = cJSON_CreateNumber(123.5);
    cJSON_AddItemToArray(items, num_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str_item);

    // step 3: Operate and Validate
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_AddNumberToObject(root, "print_success", (double)printed_ok);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(root, "second_value", second_value);
    const char *retrieved_str = cJSON_GetStringValue(cJSON_GetArrayItem(items, 2));
    cJSON *copied_str = cJSON_CreateString(retrieved_str);
    cJSON_AddItemToObject(root, "copied_str", copied_str);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}