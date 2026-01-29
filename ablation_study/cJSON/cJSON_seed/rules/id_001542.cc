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
//<ID> 1542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    const char *first_str = NULL;

    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "myarray");
    str_item = cJSON_CreateString("element_one");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddItemReferenceToObject(root, "myarray_ref", arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_item);
    printed = cJSON_PrintUnformatted(root);
    (void)first_str;
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}