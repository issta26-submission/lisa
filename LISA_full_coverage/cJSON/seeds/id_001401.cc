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
//<ID> 1401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {7, 8, 9};
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON *str_item = cJSON_CreateString("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    (void)flag_item;

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetObjectItemCaseSensitive(root, "number");
    cJSON *got_arr = cJSON_GetObjectItemCaseSensitive(root, "ints");
    cJSON *got_str = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    double num_value = cJSON_GetNumberValue(got_num);
    cJSON *first_int = cJSON_GetArrayItem(got_arr, 0);
    double first_int_value = cJSON_GetNumberValue(first_int);
    const char *greeting = cJSON_GetStringValue(got_str);
    (void)num_value;
    (void)first_int_value;
    (void)greeting;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}