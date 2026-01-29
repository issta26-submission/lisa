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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *dup_num = NULL;
    cJSON *retrieved_numbers = NULL;
    cJSON *retrieved_value = NULL;
    char *printed = NULL;
    int *numbers = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    numbers = (int *)cJSON_malloc(sizeof(int) * 3);
    numbers[0] = 7;
    numbers[1] = 8;
    numbers[2] = 9;
    arr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str);
    dup_num = cJSON_Duplicate(num, 0);

    // step 3: Operate / Validate
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    is_array = cJSON_IsArray(retrieved_numbers);
    retrieved_value = cJSON_GetObjectItem(root, "value");
    cmp_result = cJSON_Compare(retrieved_value, dup_num, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(numbers);
    cJSON_Delete(dup_num);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}