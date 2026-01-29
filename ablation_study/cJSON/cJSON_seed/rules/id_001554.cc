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
//<ID> 1554
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
    cJSON *first = NULL;
    cJSON *dup_str = NULL;
    cJSON *newobj = NULL;
    cJSON *status_item = NULL;
    char *name_buf = NULL;
    const char label[] = "copied_name";
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);

    // allocate temporary buffer via cJSON_malloc and populate it
    name_buf = (char *)cJSON_malloc(sizeof(label));
    memset(name_buf, 0, sizeof(label));
    memcpy(name_buf, label, sizeof(label));

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    dup_str = cJSON_Duplicate(first, 1);
    equal = cJSON_Compare(first, dup_str, 1);
    newobj = cJSON_CreateObject();
    status_item = cJSON_AddStringToObject(newobj, "status", "replaced");
    cJSON_ReplaceItemViaPointer(root, arr, newobj);
    cJSON_AddStringToObject(root, name_buf, cJSON_GetStringValue(first));
    cJSON_free(name_buf);
    name_buf = NULL;

    // step 4: Cleanup
    cJSON_Delete(dup_str);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}