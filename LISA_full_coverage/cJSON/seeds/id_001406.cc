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
//<ID> 1406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    int ints[] = {7, 8, 9};
    cJSON *int_arr = cJSON_CreateIntArray(ints, 3);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "text", str_item);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(3.14159);
    cJSON_ReplaceItemInObject(root, "number", new_num);
    cJSON *dup_ints = cJSON_Duplicate(int_arr, 1);
    cJSON_ReplaceItemInObject(root, "ints", dup_ints);
    cJSON_AddStringToObject(root, "note", "configured");

    // step 3: Operate & Validate
    const cJSON *got_num = cJSON_GetObjectItem(root, "number");
    const cJSON *got_ints = cJSON_GetObjectItem(root, "ints");
    int int_count = cJSON_GetArraySize(got_ints);
    const cJSON *first_int = cJSON_GetArrayItem(got_ints, 0);
    double first_int_value = cJSON_GetNumberValue(first_int);
    const cJSON *got_text = cJSON_GetObjectItem(root, "text");
    const char *text_value = cJSON_GetStringValue(got_text);
    (void)got_num;
    (void)int_count;
    (void)first_int_value;
    (void)text_value;
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