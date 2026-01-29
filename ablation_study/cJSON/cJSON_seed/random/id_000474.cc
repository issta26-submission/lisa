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
//<ID> 474
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
    cJSON *copy_str = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    const char *sval = (const char *)0;
    cJSON_bool israw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "list");
    cJSON_AddNullToObject(root, "nil");

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    israw = cJSON_IsRaw(first);
    sval = cJSON_GetStringValue(first);
    copy_str = cJSON_CreateString(sval);
    cJSON_AddItemToObject(root, "copy", copy_str);

    // step 5: Validate
    (void)israw;
    (void)sval;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}