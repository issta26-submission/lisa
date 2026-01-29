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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    char *json_str = (char *)0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    container = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToArray(container, arr_ref);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(container, 0);
    json_str = cJSON_PrintUnformatted(retrieved);
    inner = cJSON_GetArrayItem(retrieved, 0);

    // step 5: Validate
    (void)inner;
    (void)json_str;

    // step 6: Cleanup
    if (json_str) { cJSON_free(json_str); }
    cJSON_Delete(container);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}