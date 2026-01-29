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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *s_beta = cJSON_CreateString("beta");
    cJSON *s_insert = cJSON_CreateString("middle");
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *item3 = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON_bool cmp_result = 0;
    cJSON_bool isstr0 = 0;
    cJSON_bool isstr1 = 0;
    cJSON_bool isstr2 = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddItemToArray(array, s_alpha);
    cJSON_AddItemToArray(array, s_beta);
    cJSON_InsertItemInArray(array, 1, s_insert);
    cJSON_AddItemReferenceToArray(array, s_alpha);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(array, 0);
    item1 = cJSON_GetArrayItem(array, 1);
    item2 = cJSON_GetArrayItem(array, 2);
    item3 = cJSON_GetArrayItem(array, 3);
    cmp_result = cJSON_Compare(item0, item3, 1);
    isstr0 = cJSON_IsString(item0);
    isstr1 = cJSON_IsString(item1);
    isstr2 = cJSON_IsString(item2);
    buffer[0] = (char)('0' + (int)cmp_result);
    buffer[1] = (char)('0' + (int)isstr0);
    buffer[2] = (char)('0' + (int)isstr1);
    buffer[3] = cJSON_GetStringValue(item1)[0];
    buffer[4] = version[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}