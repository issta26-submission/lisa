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
//<ID> 1404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(ints, 4);
    cJSON *number_item = cJSON_CreateNumber(3.1415);
    cJSON *string_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "pi", number_item);
    cJSON_AddItemToObject(root, "msg", string_item);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(number_item, 42.0);
    cJSON *dup_array = cJSON_Duplicate(int_array, 1);
    cJSON_ReplaceItemInObject(root, "ints", dup_array);
    (void)new_val;

    // step 3: Operate & Validate
    cJSON *got_ints = cJSON_GetObjectItemCaseSensitive(root, "ints");
    int size = cJSON_GetArraySize(got_ints);
    cJSON *third = cJSON_GetArrayItem(got_ints, 2);
    double third_val = cJSON_GetNumberValue(third);
    const char *got_msg = cJSON_GetStringValue(cJSON_GetObjectItem(root, "msg"));
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)size;
    (void)third_val;
    (void)got_msg;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}