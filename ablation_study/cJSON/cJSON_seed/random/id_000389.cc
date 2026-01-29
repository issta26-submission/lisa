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
//<ID> 389
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
    char *printed = (char *)0;
    cJSON_bool raw_check = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure
    cJSON_AddItemToArray(arr, raw_item);

    // step 4: Operate
    printed = cJSON_PrintBuffered((const cJSON *)arr, 64, 1);
    raw_check = cJSON_IsRaw((const cJSON *const)raw_item);

    // step 5: Validate
    (void)printed;
    (void)raw_check;

    // step 6: Cleanup
    if (printed) { cJSON_free((void *)printed); }
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}