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
//<ID> 1400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    int nums[] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 2: Configure
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_value = cJSON_GetNumberValue(pi_item);
    cJSON *ints_item = cJSON_GetObjectItem(root, "ints");
    int ints_count = cJSON_GetArraySize(ints_item);
    cJSON *second_int_item = cJSON_GetArrayItem(ints_item, 1);
    double second_int_value = cJSON_GetNumberValue(second_int_item);
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    const char *greeting_value = cJSON_GetStringValue(greet_item);
    (void)pi_value;
    (void)ints_count;
    (void)second_int_value;
    (void)greeting_value;

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    if (printed_len > 0) {
        memcpy(buf, printed, printed_len + 1);
    }
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}