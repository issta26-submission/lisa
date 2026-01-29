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
//<ID> 2365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[3];
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON *extra_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(int_array, extra_num);
    cJSON_AddItemToObject(root, "ints", int_array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)pi_val;
    buffer[2] = flag_true ? '1' : '0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}