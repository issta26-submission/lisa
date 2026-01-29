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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *input = "{\"title\":\"test\",\"meta\":{\"count\":0}}";
    cJSON *root = cJSON_Parse(input);

    // step 2: Configure
    cJSON *array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(cfg, "enabled", flag);

    // step 3: Operate
    cJSON *num2 = cJSON_CreateNumber(3.75);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "summary", cJSON_CreateString("created"));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    int count = cJSON_GetArraySize(array);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double second_val = cJSON_GetNumberValue(second);
    (void)count;
    (void)second_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}