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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parent = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    parent = cJSON_CreateArray();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToArray(parent, ref);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(parent, 0);
    printed = cJSON_PrintUnformatted(retrieved);

    // step 5: Validate
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parent);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}