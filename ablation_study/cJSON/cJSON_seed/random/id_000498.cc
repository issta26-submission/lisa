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
//<ID> 498
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
    cJSON *got = (cJSON *)0;
    double extracted_val = 0.0;
    cJSON_bool print_ok = 0;
    char buffer[256];
    const int buf_len = (int)sizeof(buffer);

    // step 2: Initialize
    memset(buffer, 0, buf_len);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(-7.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    (void)pi_item;

    // step 4: Operate
    got = cJSON_GetArrayItem(arr, 0);
    extracted_val = cJSON_GetNumberValue(got);

    // step 5: Validate
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)extracted_val;
    (void)print_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}