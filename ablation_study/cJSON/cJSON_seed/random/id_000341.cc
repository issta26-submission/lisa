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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *outer = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *elem = (cJSON *)0;
    cJSON *outer_first = (cJSON *)0;
    cJSON *inner_first = (cJSON *)0;
    char *unformatted = (char *)0;
    char *strval = (char *)0;
    cJSON_bool added = (cJSON_bool)0;

    // step 2: Initialize
    outer = cJSON_CreateArray();
    inner = cJSON_CreateArray();
    elem = cJSON_CreateString("hello");

    // step 3: Configure
    added = cJSON_AddItemToArray(inner, elem);
    ref = cJSON_CreateArrayReference(inner);
    added = cJSON_AddItemToArray(outer, ref);
    added = cJSON_AddItemToArray(outer, cJSON_CreateString("world"));

    // step 4: Operate
    unformatted = cJSON_PrintUnformatted(outer);
    outer_first = cJSON_GetArrayItem(outer, 0);
    inner_first = cJSON_GetArrayItem(outer_first, 0);
    strval = cJSON_GetStringValue(inner_first);

    // step 5: Validate
    (void)added;
    (void)strval;
    (void)outer_first;
    (void)inner_first;

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(outer);
    cJSON_Delete(inner);

    // API sequence test completed successfully
    return 66;
}