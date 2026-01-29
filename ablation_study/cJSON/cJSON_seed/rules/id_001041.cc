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
//<ID> 1041
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
    cJSON *elem_str = NULL;
    cJSON *elem_num = NULL;
    cJSON *got_first = NULL;
    char *printed = NULL;
    const char *title = "Example Document";
    const char *author = "tester";
    const char *first_value = NULL;
    double value_num = 42.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", title);
    cJSON_AddStringToObject(root, "author", author);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    elem_str = cJSON_CreateString("first_item");
    cJSON_AddItemToArray(arr, elem_str);
    elem_num = cJSON_CreateNumber(value_num);
    cJSON_AddItemToArray(arr, elem_num);

    // step 3: Operate / Validate
    got_first = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(got_first);
    cJSON_AddStringToObject(root, "first_copy", first_value);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}