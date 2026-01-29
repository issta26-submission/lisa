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
//<ID> 1559
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *retrieved0 = NULL;
    cJSON *newstr = NULL;
    char *copied = NULL;
    const char *val0 = NULL;
    size_t len = 0;
    cJSON_bool cmp = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);

    // step 3: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetStringValue(retrieved0);
    len = (val0 != NULL) ? (strlen(val0) + 1) : 1;
    copied = (char *)cJSON_malloc(len);
    memset(copied, 0, len);
    if (val0) memcpy(copied, val0, len);
    newstr = cJSON_CreateString(copied);
    cmp = cJSON_Compare(retrieved0, newstr, 1);
    replaced = cJSON_ReplaceItemViaPointer(arr, item1, newstr);
    cJSON_AddStringToObject(root, "copied_value", copied);

    // step 4: Cleanup
    cJSON_free(copied);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}