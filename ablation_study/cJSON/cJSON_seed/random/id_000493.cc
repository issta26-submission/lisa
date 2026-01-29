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
//<ID> 493
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
    cJSON *num_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    int arr_size = 0;
    double v = 0.0;
    char buffer[256];
    const int buf_len = 256;
    cJSON_bool print_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate
    memset(buffer, 0, buf_len);
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, 2);
    v = cJSON_GetNumberValue(got_item);
    (void)print_ok;
    (void)buffer;
    (void)arr_size;
    (void)v;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}