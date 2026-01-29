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
//<ID> 387
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
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool raw_check = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    raw_item = cJSON_CreateRaw("{\"embedded\":123}");

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);

    // step 4: Operate
    raw_check = cJSON_IsRaw(raw_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate
    (void)raw_check;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}