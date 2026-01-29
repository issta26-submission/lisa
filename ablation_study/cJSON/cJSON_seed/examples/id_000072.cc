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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *parsed_label_item = cJSON_GetObjectItem(parsed, "label");
    char *parsed_label = cJSON_GetStringValue(parsed_label_item);
    cJSON *label_copy = cJSON_CreateString(parsed_label);
    cJSON_AddItemToObject(parsed, "label_copy", label_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}