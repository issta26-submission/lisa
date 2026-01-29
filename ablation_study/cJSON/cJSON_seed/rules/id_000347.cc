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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *dup_s1 = NULL;
    char *printed = NULL;
    cJSON_bool is_str = 0;
    cJSON_bool cmp = 0;
    const char *s1_val = NULL;
    const char *printed_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddItemToArray(array, s1);
    cJSON_InsertItemInArray(array, 0, s2);
    dup_s1 = cJSON_Duplicate(s1, 1);
    cJSON_AddItemReferenceToArray(array, dup_s1);

    // step 3: Operate / Validate
    s1_val = cJSON_GetStringValue(s1);
    printed = cJSON_PrintUnformatted(root);
    printed_val = printed;
    is_str = cJSON_IsString(cJSON_GetArrayItem(array, 0));
    cmp = cJSON_Compare(s1, cJSON_GetArrayItem(array, 2), 1);
    buffer[0] = s1_val ? s1_val[0] : 0;
    buffer[1] = printed_val ? printed_val[0] : 0;
    buffer[2] = (char)('0' + (int)is_str);
    buffer[3] = (char)('0' + (int)cmp);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}