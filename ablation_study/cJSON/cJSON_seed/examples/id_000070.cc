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
//<ID> 70
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
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *num_a = cJSON_CreateNumber(3.5);
    cJSON *num_b = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(numbers, num_a);
    cJSON_AddItemToArray(numbers, num_b);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}