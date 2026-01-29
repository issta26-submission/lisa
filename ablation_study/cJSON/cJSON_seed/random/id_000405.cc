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
//<ID> 405
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
    cJSON *bool_item = (cJSON *)0;
    const char *extracted = (const char *)0;
    char jsonbuf[] = " { \"greeting\" : \"hello\", \"number\" : 1 } ";
    char *minified = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, cJSON_CreateString("world"));
    cJSON_AddItemToObject(root, "items", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    cJSON_Minify(jsonbuf);
    minified = jsonbuf;

    // step 5: Validate
    extracted = cJSON_GetStringValue(got_item);
    (void)extracted;
    (void)minified;
    (void)bool_item;

    // step 6: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}