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
//<ID> 1550
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
    cJSON *first = NULL;
    cJSON *newstr = NULL;
    const char *orig = NULL;
    char *dup_str = NULL;
    size_t len = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    orig = cJSON_GetStringValue(first);
    len = (orig != NULL) ? (strlen(orig) + 1) : 1;
    dup_str = (char *)cJSON_malloc(len);
    memcpy(dup_str, orig ? orig : "", len);
    newstr = cJSON_CreateString(dup_str);
    cJSON_ReplaceItemViaPointer(arr, first, newstr);
    cmp_result = cJSON_Compare(cJSON_GetArrayItem(arr, 0), newstr, 1);
    (void)cmp_result;
    cJSON_AddStringToObject(root, "status", "replaced");

    // step 4: Cleanup
    cJSON_free(dup_str);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}