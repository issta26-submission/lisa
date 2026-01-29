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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure and populate
    cJSON *num0 = cJSON_CreateNumber(1.5);
    cJSON_InsertItemInArray(numbers, 0, num0);
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON_InsertItemInArray(numbers, 1, num1);
    cJSON *direct_num = cJSON_AddNumberToObject(root, "direct_value", 3.0);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_false", flag_false);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(numbers);
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double v_first = cJSON_GetNumberValue(first);
    double v_second = cJSON_GetNumberValue(second);
    double v_direct = cJSON_GetNumberValue(direct_num);
    cJSON *t_item = cJSON_GetObjectItem(root, "flag_true");
    cJSON_bool t_val = cJSON_IsTrue(t_item);
    double computed_sum = v_first + v_second + v_direct + (double)(t_val * 10);
    (void)size;
    (void)computed_sum;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}