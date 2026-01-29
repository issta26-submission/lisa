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
//<ID> 1407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    int ints[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(ints, 3);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "message", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *backup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetObjectItem(root, "num");
    double num_value = cJSON_GetNumberValue(got_num);
    cJSON *got_array = cJSON_GetObjectItem(root, "ints");
    int array_size = cJSON_GetArraySize(got_array);
    cJSON *first_elem = cJSON_GetArrayItem(got_array, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    cJSON *got_str = cJSON_GetObjectItem(root, "message");
    const char *str_value = cJSON_GetStringValue(got_str);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)num_value;
    (void)array_size;
    (void)first_value;
    (void)str_value;

    // step 4: Cleanup
    cJSON_Delete(backup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}