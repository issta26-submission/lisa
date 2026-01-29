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
//<ID> 1500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"name\":\"Bob\",\"numbers\":[10,20,30]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, sizeof(json_text) - 1, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    cJSON *numbers_item = cJSON_GetObjectItem(parsed, "numbers");

    // step 2: Configure
    int extra_vals[] = {7, 8, 9, 11};
    cJSON *int_arr = cJSON_CreateIntArray(extra_vals, 4);
    cJSON_AddItemToObjectCS(root, "int_array_cs", int_arr);
    cJSON_AddItemReferenceToObject(root, "orig_numbers", numbers_item);
    cJSON *detached_numbers = cJSON_DetachItemFromObject(parsed, "numbers");

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *name_str = cJSON_GetStringValue(name_item);
    int size_int_arr = cJSON_GetArraySize(int_arr);
    int size_detached = cJSON_GetArraySize(detached_numbers);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    (void)name_str;
    (void)size_int_arr;
    (void)size_detached;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}