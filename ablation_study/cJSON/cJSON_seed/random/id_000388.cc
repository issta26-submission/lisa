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
//<ID> 388
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
    cJSON *raw_item = (cJSON *)0;
    cJSON *elem = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw = 0;
    int prebuffer = 256;
    cJSON_bool fmt = 1;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    elem = cJSON_GetArrayItem(arr, 0);
    is_raw = cJSON_IsRaw(elem);
    printed = cJSON_PrintBuffered(root, prebuffer, fmt);

    // step 5: Validate
    (void)is_raw;
    (void)elem;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}