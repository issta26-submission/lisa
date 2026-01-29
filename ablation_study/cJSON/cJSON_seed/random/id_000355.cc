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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *real_array = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *out = (char *)0;
    cJSON_bool arr_check = (cJSON_bool)0;
    cJSON_bool raw_is_arr = (cJSON_bool)0;
    int size = 0;
    double val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    real_array = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "actual_array", real_array);
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(real_array, num1);
    cJSON_AddItemToArray(real_array, num2);
    raw_item = cJSON_AddRawToObject(root, "embedded_raw", "{\"nested\": [1, 2, 3]}");

    // step 4: Operate
    arr_check = cJSON_IsArray(real_array);
    size = cJSON_GetArraySize(real_array);
    first_elem = cJSON_GetArrayItem(real_array, 0);
    val = cJSON_GetNumberValue(first_elem);
    out = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)arr_check;
    (void)size;
    (void)val;
    raw_is_arr = cJSON_IsArray(raw_item);
    (void)raw_is_arr;
    cJSON_GetObjectItem(root, "embedded_raw");
    (void)first_elem;

    // step 6: Cleanup
    if (out) { cJSON_free(out); out = (char *)0; }
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}