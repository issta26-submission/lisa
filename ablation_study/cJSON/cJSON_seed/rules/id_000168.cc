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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.5);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddStringToObject(root, "label", "sample");

    // step 3: Operate / Validate
    int array_size = cJSON_GetArraySize(numbers);
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    const char *label_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    (void)array_size;
    (void)first_value;
    (void)label_value;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}