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
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    cJSON *answer_item = (cJSON *)0;
    char buffer[256];
    int buf_len = (int)sizeof(buffer);
    cJSON_bool pre_ok = 0;
    int arr_size = 0;
    double answer_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    added_num = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate
    memset(buffer, 0, (size_t)buf_len);
    pre_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    arr_size = cJSON_GetArraySize(arr);
    answer_item = cJSON_GetObjectItem(root, "answer");
    answer_val = cJSON_GetNumberValue(answer_item);

    // step 5: Validate
    (void)pre_ok;
    (void)arr_size;
    (void)answer_val;
    (void)added_num;
    (void)buffer;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}