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
//<ID> 1081
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
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    char *got_str = NULL;
    cJSON_bool add_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    false_item = cJSON_CreateFalse();
    add_ok = cJSON_AddItemToArray(arr, false_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(got_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}