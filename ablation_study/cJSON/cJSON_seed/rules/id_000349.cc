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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *s_beta = cJSON_CreateString("beta");
    cJSON *num = cJSON_CreateNumber(42);
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    char *printed = NULL;
    cJSON_bool is_str0 = 0;
    cJSON_bool cmp0 = 0;
    cJSON_bool cmp2 = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "first", s_alpha);
    cJSON_AddItemReferenceToArray(arr, s_alpha);
    cJSON_AddItemToArray(arr, s_beta);
    cJSON_InsertItemInArray(arr, 1, num);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    is_str0 = cJSON_IsString(item0);
    cmp0 = cJSON_Compare(item0, s_alpha, 1);
    cmp2 = cJSON_Compare(item2, s_beta, 1);
    printed = cJSON_PrintUnformatted(root);
    const char *s0 = cJSON_GetStringValue(item0);
    const char *s2 = cJSON_GetStringValue(item2);
    buffer[0] = (char)('0' + (int)is_str0);
    buffer[1] = (char)('0' + (int)cmp0);
    buffer[2] = (char)('0' + (int)cmp2);
    buffer[3] = s0[0];
    buffer[4] = s2[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}