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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *buffer = (char *)0;
    cJSON_bool is_raw = 0;
    int validation_sum = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    num_item = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate
    buffer = cJSON_PrintBuffered((const cJSON *)arr, 128, 1);
    is_raw = cJSON_IsRaw(raw_item);

    // step 5: Validate
    validation_sum = (buffer != (char *)0) + (is_raw ? 1 : 0);
    (void)validation_sum;

    // step 6: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}