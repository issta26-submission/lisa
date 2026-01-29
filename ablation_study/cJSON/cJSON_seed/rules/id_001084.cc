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
//<ID> 1084
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    char *str_val = NULL;
    cJSON_bool add_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    bool_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "list", arr);
    add_res = cJSON_AddItemToArray(arr, bool_item);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "name");
    str_val = cJSON_GetStringValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}