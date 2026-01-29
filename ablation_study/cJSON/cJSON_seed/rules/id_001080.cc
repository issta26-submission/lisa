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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *arr = NULL;
    cJSON *initial_str = NULL;
    cJSON *copied_str = NULL;
    cJSON *false_item = NULL;
    cJSON *got_obj = NULL;
    cJSON *got_key = NULL;
    char *printed = NULL;
    char *strval = NULL;
    cJSON_bool add_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    initial_str = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(obj, "greeting", initial_str);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "metadata", obj);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    got_obj = cJSON_GetObjectItem(root, "metadata");
    got_key = cJSON_GetObjectItem(got_obj, "greeting");
    strval = cJSON_GetStringValue(got_key);
    copied_str = cJSON_CreateString(strval);
    add_res = cJSON_AddItemToArray(arr, copied_str);
    false_item = cJSON_CreateFalse();
    add_res = cJSON_AddItemToArray(arr, false_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}