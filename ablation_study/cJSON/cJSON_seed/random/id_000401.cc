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
//<ID> 401
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
    cJSON *got_item = (cJSON *)0;
    const char *extracted = (const char *)0;
    char json[] = " { \"hello\" : \"world\" , \"unused\" : false } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    cJSON_Minify(json);

    // step 5: Validate
    extracted = cJSON_GetStringValue(got_item);
    (void)extracted;

    // step 6: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}