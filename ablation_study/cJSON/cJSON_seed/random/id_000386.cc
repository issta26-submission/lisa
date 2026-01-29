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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *array = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool raw_flag = 0;
    int arr_size = 0;
    const char *raw_json = "{\"embedded\":true}";

    // step 2: Initialize
    array = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw(raw_json);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, str_item);

    // step 4: Operate
    printed = cJSON_PrintBuffered((const cJSON *)array, 128, 1);
    raw_flag = cJSON_IsRaw((const cJSON *const)raw_item);
    arr_size = cJSON_GetArraySize((const cJSON *)array);

    // step 5: Validate
    got0 = cJSON_GetArrayItem(array, 0);
    got1 = cJSON_GetArrayItem(array, 1);
    (void)got0;
    (void)got1;
    (void)raw_flag;
    (void)arr_size;

    // step 6: Cleanup
    if (printed) cJSON_free((void *)printed);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}