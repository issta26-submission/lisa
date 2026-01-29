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
//<ID> 1236
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
    cJSON *true_item = (cJSON *)0;
    char *printed = (char *)0;
    int printed_nonnull = 0;
    cJSON_bool is_raw_flag = 0;
    int validation = 0;

    // step 2: Setup - create root object and an array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a raw item and a true boolean, add both to the array and attach to root
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - produce a buffered print of the root and inspect the raw item flag
    printed = cJSON_PrintBuffered(root, 256, 1);
    printed_nonnull = (printed != (char *)0);
    is_raw_flag = cJSON_IsRaw(raw_item);
    validation = printed_nonnull + (int)is_raw_flag + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0);

    // step 5: Cleanup - free printed buffer and delete the root (which frees attached children)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation;
    return 66;
}