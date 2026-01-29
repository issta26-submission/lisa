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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.718);
    cJSON *num3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, num3);
    cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *parsed_array = cJSON_GetObjectItem(parsed, "values");
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *incremented = cJSON_CreateNumber(first_val + 1.0);
    cJSON_AddItemToObject(root, "incremented", incremented);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}