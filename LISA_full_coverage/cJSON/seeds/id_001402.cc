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
//<ID> 1402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "text", str_item);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    cJSON *replacement_number = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemInObject(root, "number", replacement_number);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *got_ints = cJSON_GetObjectItem(root, "ints");
    int arr_size = cJSON_GetArraySize(got_ints);
    cJSON *first_int = cJSON_GetArrayItem(got_ints, 0);
    double first_val = cJSON_GetNumberValue(first_int);
    cJSON *got_text = cJSON_GetObjectItem(root, "text");
    const char *text_val = cJSON_GetStringValue(got_text);
    (void)arr_size;
    (void)first_val;
    (void)text_val;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}