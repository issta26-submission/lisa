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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str = cJSON_CreateString("hello world");
    cJSON *tru = cJSON_CreateTrue();
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int buf_len = 256;
    cJSON_bool printed_ok = 0;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    char *sval = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(obj, "greeting", str);
    cJSON_AddItemReferenceToArray(arr, str);
    cJSON_AddItemToArray(arr, tru);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "container", obj);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    sval = cJSON_GetStringValue(item0);
    is_true = cJSON_IsTrue(item1);
    (void)printed_ok;
    (void)sval;
    (void)is_true;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}