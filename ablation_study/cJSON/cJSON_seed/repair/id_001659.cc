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
//<ID> 1659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers_vals[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers_vals, 3);
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToArray(container, cJSON_CreateString("alpha"));

    // step 2: Configure
    cJSON_AddItemToArray(container, cJSON_CreateNumber(42));
    cJSON_AddItemToArray(container, cJSON_CreateString("beta"));

    // step 3: Operate and Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *second_item = cJSON_GetArrayItem(int_array, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)has_numbers);
    buffer[1] = (char)('0' + (int)second_value % 10);
    buffer[2] = printed ? printed[0] : 'X';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}