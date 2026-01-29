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
//<ID> 1503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "greeting", "hello");
    cJSON_AddItemToObject(root, "child", child);
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObjectCS(root, "ints", int_array);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, (size_t)1024, &parse_end, 1);

    // step 3: Operate & Validate
    cJSON *ints_item = cJSON_GetObjectItem(root, "ints");
    int array_size = cJSON_GetArraySize(ints_item);
    cJSON *first_item = cJSON_GetArrayItem(ints_item, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    const cJSON *greet_item = cJSON_GetObjectItem(child, "greeting");
    const char *greet_value = cJSON_GetStringValue(greet_item);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)array_size;
    (void)first_value;
    (void)greet_value;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}