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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added = 0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure
    added = cJSON_AddItemToArray(root, raw_item);

    // step 4: Operate
    printed = cJSON_PrintBuffered((const cJSON *)root, 64, 1);

    // step 5: Validate
    first = cJSON_GetArrayItem(root, 0);
    is_raw = cJSON_IsRaw((const cJSON *const)first);
    (void)added;
    (void)is_raw;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}