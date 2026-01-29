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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *val_true = cJSON_CreateTrue();
    cJSON *val_str = cJSON_CreateString("example");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int buf_len = 128;
    int arr_size = 0;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    char *sval = NULL;
    cJSON_bool added_true = 0;
    cJSON_bool added_ref = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    added_true = cJSON_AddItemToArray(arr, val_true);
    added_ref = cJSON_AddItemReferenceToArray(arr, val_str);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    sval = cJSON_GetStringValue(item1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)item0;
    (void)item1;
    (void)sval;
    (void)added_true;
    (void)added_ref;
    (void)buf_len;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}