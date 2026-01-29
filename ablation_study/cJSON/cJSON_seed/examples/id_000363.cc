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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const colors[] = { "red", "green", "blue" };
    cJSON *str_array = cJSON_CreateStringArray(colors, 3);
    cJSON_AddItemToObject(root, "colors", str_array);
    cJSON *note = cJSON_CreateString("initialized");
    cJSON_AddItemToObject(root, "note", note);

    // step 2: Configure
    char *temp_buf = (char *)cJSON_malloc(16);
    memset(temp_buf, 0, 16);
    temp_buf[0] = 'o'; temp_buf[1] = 'k'; temp_buf[2] = '\0';
    cJSON *frombuf = cJSON_CreateString(temp_buf);
    cJSON_AddItemToObject(root, "frombuf", frombuf);
    cJSON_free(temp_buf);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);

    // step 3: Operate & Validate
    cJSON_bool root_is_object = cJSON_IsObject(root);
    (void)root_is_object;
    cJSON_bool null_check = cJSON_IsNull(maybe_null);
    (void)null_check;
    int count = cJSON_GetArraySize(str_array);
    cJSON *second = cJSON_GetArrayItem(str_array, 1);
    const char *second_val = cJSON_GetStringValue(second);
    cJSON *picked = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(root, "picked", picked);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}